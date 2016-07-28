import logging
import sys
import SocketServer
import serial
import os
import time
import threading

MAX_LENGTH = 256
PORT = 10000
HOST = '127.0.0.1'

logging.basicConfig(level=logging.DEBUG,
                    format = '%(asctime)s %(name)s %(levelname)s %(message)s',
                    filename = "../logging/MotorControllerServer.log",
                    filemode = 'w')

class SerialRequestHandler(SocketServer.BaseRequestHandler):

    def __init__(self, request, client_address, server):
        self.logger = logging.getLogger('SerialRequestHandler')
        self.current_thread = " " + threading.current_thread().name
        self.logger.debug("__init__" + self.current_thread)
        self.server = server
        SocketServer.BaseRequestHandler.__init__(self, request, client_address, server)
        return

    def setup(self):
        self.logger.debug("setup" + self.current_thread)
        try:
            self.ino = serial.Serial("COM8", 9600, timeout=30)
            while not self.ino.writable():
                self.logger.warning("Device not writable!")
        except serial.SerialException:
            self.logger.warning("No Arduino found or incorrect port!")
            self.no_ino = True
        return SocketServer.BaseRequestHandler.setup(self)

    def handle(self):
        valid = ['calibrate', 'step', 'distance', 'home', 'debug', 'close']
        self.logger.debug("handle" + self.current_thread)
        cmd = self.request.recv(MAX_LENGTH)
        if cmd.split(" ")[0] in valid:
            if cmd.split(" ")[0] == 'close':
                self.logger.debug("Closing server")
            elif self.no_ino:
                self.logger.debug("Received command " + cmd)
                self.request.send("ACK NO EXEC " + cmd)
            else:
                self.ino.write(cmd)
                self.logger.debug("Sent " + cmd + " to Arduino")
                while self.ino.inWaiting() > 0:
                    line = self.ino.readline()
                    if line[:8] == 'LOGGING':
                        self.logger.debug(line)
                    else:
                        print line
                self.request.send("ACK " + cmd)
        else:
            reply = "Invalid Request - Valid = {'calibrate <axis length>', 'step <steps>', 'distance <dist>', 'home', 'debug <steps> - use sparingly!'}"
            self.request.send(reply)
            self.logger.warning("Request: " + cmd)
            self.logger.warning("INVALID REQUEST")

        return

    def finish(self):
        self.logger.debug('finish' + self.current_thread)
        return SocketServer.BaseRequestHandler.finish(self)

class SerialServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):

    keep_going = True

    def __init__(self, server_address, handler_class=SerialRequestHandler):
        self.logger = logging.getLogger('SerialServer')
        SocketServer.TCPServer.__init__(self, server_address, handler_class)
        return

    def server_activate(self):
        self.logger.debug('server_activate')
        SocketServer.TCPServer.server_activate(self)
        return

    def serve_forever(self):
        self.logger.info('Handling requests')
        while True:
            self.handle_request()
        return

    def verify_request(self, request, client_address):
        print "Got request from ", client_address
        return SocketServer.TCPServer.verify_request(self, request, client_address)

    def handle_request(self):
        self.logger.debug('handle_request')
        if self.keep_going:
            return SocketServer.TCPServer.handle_request(self)
        else:
            print "Done - closing up"
            self.shutdown()
            self.server_close()

    def finish_request(self, request, client_address):
        print "Finished request from ", client_address, "...Terminating Connection"
        return SocketServer.TCPServer.finish_request(self, request, client_address)

    def server_close(self):
        self.logger.debug('server_close')
        return SocketServer.TCPServer.server_close(self)


class ServerThread(threading.Thread):
    def run(self, target):
        while True:
            target.handle_request()
        target.shutdown()
        target.server_close()
        return




def main():

    address = (HOST,PORT)
    server = SerialServer(address, SerialRequestHandler)
    ip, port = server.server_address
    print "Server active on {}:{}".format(ip, port)
    t = ServerThread()
    t.setDaemon(True)
    t.run(target=server)


if __name__ == "__main__":
    main()
