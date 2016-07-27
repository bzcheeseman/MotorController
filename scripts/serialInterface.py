from serial import Serial
import sys
import SocketServer
from multiprocessing import Process, Lock
import os
from time import strftime

MAX_LENGTH = 4096
PORT = 10000
HOST = '127.0.0.1'

def _log_int(text, timeatopen):
    try:
        os.chdir("../logging")
    except:
        os.mkdir("../logging")

    try:
        f = open("log_"+timeatopen+".txt", "a+", 0)
    except IOError:
        file = os.open("log_"+timeatopen+".txt", os.O_RDWR)
        f = os.fdopen(file)

    f.write(text)
    f.close()
    os.chdir("../scripts")
    return

def send_command(command, timeatopen):
    _log_int("Sent command " + command + " to Arduino\n", timeatopen)

    serial = Serial("COM8", 9600)

    serial.write(command)
    while serial.inWaiting > 0:
        line = serial.readline()
        if line[:7] == "LOGGING":
            _log_int("Arduino:" + line[8:], timeatopen)
        else:
            print line

def endServer(timeatopen):
    _log_int("Ending server at "+timeatopen, timeatopen)
    sys.exit(0)

class tcpHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        timeatopen = strftime("%Y%m%d_%I%M")
        buf = self.request.recv(MAX_LENGTH).strip()
        if buf == "END":
            os.chdir("../scripts")
            endServer(timeatopen)
        elif buf == "cmd":
            self.request.send("Arduino Command: ")
            cmd = self.request.recv(MAX_LENGTH).strip()
            send_command(cmd, timeatopen)
        elif buf == "log":
            self.request.send("Log entry to commit: ")
            log = self.request.recv(MAX_LENGTH).strip()
            _log_int(log, timeatopen)





def main():

    server = SocketServer.TCPServer((HOST, PORT), tcpHandler)
    print "Server started at {}:{}".format(HOST, PORT)
    server.serve_forever()


if __name__ == "__main__":
    main()