import socket
import sys


HOST = '127.0.0.1'
CMD_PORT = 10000

s_cmd = socket.socket()

s_cmd.connect((HOST, CMD_PORT))


try:
    msg = raw_input("Command To Send: ")
    if msg == "close":
        s_cmd.send("END")
        s_cmd.close()
        sys.exit(0)
    else:
        s_cmd.sendall(msg + "\n")
        print s_cmd.recv(1024)
finally:
    s_cmd.close()