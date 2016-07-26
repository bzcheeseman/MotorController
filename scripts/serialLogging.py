import serial
import os
from time import strftime
import sys


'''
@file serialLogging.py
@brief Logging module for the serial port
'''


try:
    os.chdir("../logging")
except:
    os.mkdir("../logging")

port = sys.argv[1]
serial = serial.Serial(port, 9600)
value = 0

try:
    f = open("log_"+strftime("%Y%m%d_%I%M")+".txt", "w")
except IOError:
    file = os.open("log_"+strftime("%Y%m%d_%I%M")+".txt", os.O_RDWR)
    f = os.fdopen(file)
while 1:
    value = serial.read(7);
    print value
    if value == "LOGGING":
        log_string = serial.read(20*5)
        f.write(log_string)
        f.close()

