import logging
import sys
import os
import serial
import threading
import pandas as pd
import numpy as np

logging.basicConfig(level=logging.DEBUG,
                    format = '%(asctime)s %(threadName)s %(name)s %(levelname)s %(message)s',
                    filename = "../logging/serialRequestHandler.log",
                    filemode = 'w')

class arduino(object):
    def __init__(self, port, baud=9600, timeout=10):
        self.logger = logging.getLogger("Arduino")
        try:
            self.ino = serial.Serial(port, baud, timeout=timeout)
            self.logger.info("Arduino communications opened at %s" % port)
        except serial.SerialException:
            self.ino = None
            self.logger.warning("No Arduino found at port %s" % port)
            self.logger.warning("Proceeding with no Arduino!")

    def handle(self, cmd):
        valid = ['calibrate', 'step', 'distance', 'home', 'debug', 'close']

        if cmd.split(" ")[0].lower() in valid:
            to_send = cmd.lower()
            if self.ino:
                if cmd.split(" ")[0] == 'close':
                    self.ino.close()
                    return 0
                else:
                    self.ino.write(b'%s\n' % to_send)
                    self.ino.flush()
                    self.logger.info("Sent '" + to_send + "' to Arduino on " + port)
                    time.sleep(1)
                    while self.ino.in_waiting > 0:
                        line = self.ino.read(self.ino.in_waiting)
                        print line
                    return 0
            else:
                self.logger.info("Would have sent: %s" % to_send)
                self.logger.info("No Arduino connected")
                return 1
        else:
            reply = "Invalid Request - Valid = {'calibrate <axis length>', 'step <steps>', 'distance <dist>', \
            'home', 'debug <steps>' - use sparingly!}"
            self.logger.warning("Request: " + cmd)
            self.logger.warning("INVALID REQUEST")
            if self.ino:
                pass
            else:
                self.logger.info("No Arduino connected")
            return 1

def HandleCommands(arduino, commands):
    ts = []
    for command in commands:
        ts.append(threading.Thread(name=command, target=arduino.handle, args = [command]))

    for t in ts:
        t.start()

    for t in ts:
        t.join()

'''
Allows for 2 lines of header at the top of the file for description.  Assumes a .csv file of commands.
'''
def ReadCommands(filename):
    commands = []
    with open(filename, "r") as f:
        for line in f:
            commands.append(line)





if __name__ == '__main__':
    ar = arduino("/dev/tty0")
    #multiple commands
    HandleCommands(ar, ["distance 5", "sdlfkj", "debug z 400"])
    #single command
    ar.handle("debug z 1000")
