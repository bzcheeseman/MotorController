import logging
import socket
import sys
import time

MAX_LENGTH = 512
def main():
    logging.basicConfig(level=logging.DEBUG,
                        format = '%(asctime)s %(name)s %(levelname)s %(message)s',
                        filename = "../logging/MotorControllerClient.log",
                        filemode = 'w')

    ip = 'localhost'
    port = 10000

    logger = logging.getLogger('client')
    logger.info('Server on %s:%s', ip, port)

    # Connect to the server
    logger.debug('creating socket')
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    logger.debug('connecting to server')
    s.connect((ip, port))

    # Send the data
    message = 'calibrate'
    logger.debug('sending data: "%s"', message)
    len_sent = s.send(message)

    # Receive a response
    logger.debug('waiting for response')
    response = s.recv(MAX_LENGTH)
    logger.debug('response from server: "%s"', response)

    # s.close()
    #
    # Connect to the server
    logger.debug('creating socket')
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    logger.debug('connecting to server')
    s.connect((ip, port))

    # Try an invalid message
    message = 'hello world'
    logger.debug('sending data: "%s"', message)
    len_sent = s.send(message)

    # Receive a response
    logger.debug('waiting for response')
    response = s.recv(MAX_LENGTH)
    logger.debug('response from server: "%s"', response)

    s.close()

    # Connect to the server
    logger.debug('creating socket')
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    logger.debug('connecting to server')
    s.connect((ip, port))

    # close the server
    s.send("close")

    # Clean up
    logger.debug('closing socket')
    s.close()
    logger.debug('done')

if __name__ == '__main__':
    main()
