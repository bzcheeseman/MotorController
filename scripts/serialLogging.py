import serial
import os
from time import strftime


'''
@file serialLogging.py
@brief Logging module for the serial port
'''

def log(port):
    try:
        os.chdir("../logging")
    except:
        os.mkdir("../logging")

    serial = serial.Serial(port, 9600)

    try:
        f = open("log_"+strftime("%Y%m%d_%I%M")+".txt", "w")
    except IOError:
        file = os.open("log_"+strftime("%Y%m%d_%I%M")+".txt", os.O_RDWR)
        f = os.fdopen(file)

    try:
        value = serial.read(7);
        print value
        if value == "LOGGING":
            log_string = serial.read(20*5)
            f.write(log_string)
            f.close()
    except serial.SerialException:
        pass

