import serial
import os
from time import strftime
import sys

try:
    os.chdir("logging")
except:
    os.mkdir("logging")

port = sys.argv[1]
serial = serial.Serial(port, 9600)
value = 0

with open("logging/log_"+strftime("%Y%m%d_%I%M")+".txt", "a+") as f:
    while 1:
        value = serial.read(7);
        print value
        if value == "LOGGING":
            log_string = serial.read(1000)
            f.write(log_string)

