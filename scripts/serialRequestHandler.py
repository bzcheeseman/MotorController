import logging
import serial
import threading
import time

logging.basicConfig(level=logging.DEBUG,
                    format = '%(asctime)s %(threadName)s %(name)s %(levelname)s %(message)s',
                    filename = "../logging/serialRequestHandler.log",
                    filemode = 'w')

class arduino(object):
    def __init__(self, port, baud=9600, timeout=10):
        self.port = port
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
                    self.logger.info("Sent '" + to_send + "' to Arduino on " + self.port)
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
            self.logger.warning("Request: " + cmd)
            self.logger.warning("INVALID REQUEST")
            if self.ino:
                pass
            else:
                self.logger.info("No Arduino connected")
            return 1

    def run(self, cmd, lock):
        with lock:
            self.handle(cmd)

def HandleCommands(arduino, commands):
    ts = []
    l = threading.RLock()
    for command in commands:
        ts.append(threading.Thread(name=command, target=arduino.run, args = [command, l]))

    for t in ts:
        t.start()

    for t in ts:
        t.join()

'''
Allows for <header> lines of header at the top of the file for description.  Assumes a .csv file of commands.
'''
def ReadCommands(arduino, filename, header):
    commands = []
    out_commands = []
    i = 0
    with open(filename, "r") as f:
        for line in f:
            if i >= header:
                commands.append(line.strip("\n"))
            i += 1
    for command in commands:
        if command[0] == "D":
            c = command.split(" ")[1:]
            for block in c:
                axis = block[0]
                distance = block[1:]

                out_commands.append("debug {} {}".format(axis, distance))
        else:
            c = command.split(" ")
            for block in c:
                axis = block[0]
                distance = block[1:]

                out_commands.append("distance {} {}".format(axis, distance))

    HandleCommands(arduino, out_commands)





if __name__ == '__main__':
    ar = arduino("/dev/tty0")
    #multiple commands
    HandleCommands(ar, ["distance 5", "sdlfkj", "debug z 400"])
    ReadCommands(ar, "../distance_command_list/sample.txt", 0)
    #single command
    ar.handle("debug z 1000")


