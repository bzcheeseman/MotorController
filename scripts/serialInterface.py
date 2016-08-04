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

    no_ino = False
    ino = None

    def __init__(self, request, client_address, server):
        self.logger = logging.getLogger('SerialRequestHandler')
        self.current_thread = " " + threading.current_thread().name
        self.logger.debug("__init__" + self.current_thread)
        self.server = server
        SocketServer.BaseRequestHandler.__init__(self, request, client_address, server)
        return

    def setup(self):
        self.logger.debug("setup" + self.current_thread)
        return SocketServer.BaseRequestHandler.setup(self)

    def handle(self):
        valid = ['calibrate', 'step', 'distance', 'home', 'debug', 'close', 'switch', 'reset']
        self.logger.debug("handle" + self.current_thread)
        cmd = self.request.recv(MAX_LENGTH)
        
        port = ""
        if cmd.split(" ")[0][:3] == "COM": 
            port = cmd.split(" ")[0]
        elif cmd.split(" ")[0] == 'close':
            self.logger.debug("Closing server")
            return
        
        try:
            self.ino = serial.Serial(port, 9600, timeout=10)
            while not self.ino.writable():
                self.logger.warning("Device not writable!")
        except serial.SerialException:
            self.logger.warning("No Arduino found or incorrect port!")
            self.no_ino = True
        
        if cmd.split(" ")[1].lower() in valid:
            to_send = " ".join(cmd.split(" ")[1:]).lower()
            if self.no_ino:
                self.logger.debug("Received command " + to_send)
                self.request.send("ACK NO EXEC " + to_send)
            else:
                if cmd.split(" ")[1] == 'close':
                    self.ino.close()
                    return
                else:
                    print to_send
                    print self.ino.write(b'%s\n' % to_send)
                    self.ino.flush()
                    self.logger.debug("Sent '" + to_send + "' to Arduino on " + port)
                    time.sleep(1)
                    while self.ino.in_waiting > 0:
                        line = self.ino.read(self.ino.in_waiting)
                        print line
                    self.request.send("ACK " + to_send)
                    self.ino.close()
        else:
            reply = "Invalid Request - Valid = {'calibrate <axis length>', 'step <steps>', 'distance <dist>', \
            'home', 'debug <steps>' - use sparingly!, 'switch <switch>', 'reset'}"
            self.request.send(reply)
            self.logger.warning("Request: " + cmd)
            self.logger.warning("INVALID REQUEST")

        return

    def finish(self):
        self.logger.debug('finish' + self.current_thread)
        return SocketServer.BaseRequestHandler.finish(self)

class SerialServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):

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
        return SocketServer.TCPServer.handle_request(self)

    def finish_request(self, request, client_address):
        print "Responding to requests from ", client_address
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
    t.run(server)


if __name__ == "__main__":
    main()
