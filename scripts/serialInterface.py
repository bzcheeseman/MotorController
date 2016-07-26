from serialLogging import log
import serial
import sys
import time

port = sys.argv[1]
serial = serial.Serial()
serial.baudrate = 9600
serial.port = port

def main():
    while 1:
        command = raw_input("Command (and value, separated by spaces) to send: ")
        cmd = command.split(" ")

        serial.open()

        if cmd[0] == "STEPS":
            serial.write(cmd[1])
        else:
            serial.open()
            log(port, serial)
            serial.close()

if __name__ == "__main__":
    main()