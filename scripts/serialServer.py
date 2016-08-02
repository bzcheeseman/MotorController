import sys
import socket
import serial
import logging
import select
import threading
import time
import os

MAX_LENGTH = 2**5

logging.basicConfig(level=logging.DEBUG,
                    format = '%(asctime)s %(threadName)s %(name)s %(levelname)s %(message)s',
                    filename = "../logging/MotorControllerServer.log",
                    filemode = 'w')

class SerialServer():
    def __init__(self, host='localhost', port=10000, backlog=5):
        self.log = logging.getLogger('SerialServer')
        self.host = host
        self.port = port
        self.server = None
        self.backlog = backlog
        self.threads = []

    def __open__(self):
        try:
            self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server.bind((self.host, self.port))
            self.server.listen(self.backlog)
            print "Server running on {}:{}".format(self.host, self.port)
        except socket.error, (value, message):
            if self.server:
                self.server.close()
                self.log.fatal(message)
            else:
                self.log.fatal(message)
                sys.exit(1)

    def run(self):
        self.__open__()
        incoming = [self.server]
        running = 1
        while running:
            inputready, outputready, exceptready = select.select(incoming, [], [])

            for ins in inputready:
                if ins == self.server:
                    client = SerialHandler(self.server.accept())
                    self.log.info("Accepted incoming request from " + str(client.address[0]))
                    client.start()
                    self.threads.append(client)

                elif s == sys.stdin:
                    incoming = sys.stdin.readline()
                    self.log.debug("Received shutdown signal.")
                    self.log.info("Shutting down")
                    running = 0

        self.server.close()
        for thread in self.threads:
            thread.join()

        return 0

class SerialHandler(threading.Thread):
    def __init__(self,(client,address)):
        threading.Thread.__init__(self)
        self.log = logging.getLogger('SerialHandler')
        self.client = client
        self.address = address
        self.size = 128
        self.ino = None
        self.timeout = 5

    def run(self):
        running = 1
        while running:
            data = self.client.recv(self.size)
            msg = "Handling request from " + str(self.address[0])
            print msg
            self.log.info(msg)

            if data:
                self.log.debug("Received: " + data)
                self.client.send("ACK " + data)

                if data.split(" ")[0][:3] == "COM":
                    try:
                        self.ino = serial.Serial(data.split(" ")[0], 9600, timeout=self.timeout)
                        time.sleep(1)
                        to_send = " ".join(data.split(" ")[1:])

                        self.log.info("Sending to Arduino on " + self.ino.port + ": " + to_send)
                        self.ino.write(to_send + '\n')
                        time.sleep(1)

                        if os.name != "nt":
                            read,_,_ = select.select([self.ino], [], [], self.timeout)

                            while read[0]:
                                line = self.ino.readline()
                                self.log.info(line)
                                if len(line) < 2:
                                    break

                        else:
                            while self.ino.in_waiting > 0:
                                l = self.ino.read(MAX_LENGTH)
                                self.log.info(l)
                                print l
                                self.client.send(l)

                        self.ino.close()

                    except serial.SerialException, message:
                        self.log.warning(message)
                        print message

            else:
                self.client.close()
                self.log.info("Client %s closed connection" % self.address[0])
                running = 0


if __name__ == '__main__':
    s = SerialServer()
    s.run()
