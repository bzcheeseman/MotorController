from serialLogging import log
import serial
import sys

port = sys.argv[1]
serial = serial.Serial(port, 9600)

def main():
    while 1:
        command = raw_input("Command (and value, separated by spaces) to send: ")
        cmd = command.split(" ")

        if cmd[0] == "STEPS":
            serial.send(cmd[1])
        else:
            log(port)

if __name__ == "__main__":
    main()