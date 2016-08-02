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

    message = 'COM8 calibrated x'
    logger.debug('sending data: "%s"', message)
    s.send(message)

    # Receive a response
    logger.debug('waiting for response')
    response = s.recv(MAX_LENGTH)
    logger.debug('response from server: "%s"', response)

    logger.debug('waiting for response')
    responsecalx = s.recv(MAX_LENGTH)
    logger.debug('response from server: "%s"', responsecalx)

    message = 'COM8 calibrated y'
    logger.debug('sending data: "%s"', message)
    s.send(message)

    # Receive a response
    logger.debug('waiting for response')
    response = s.recv(MAX_LENGTH)
    logger.debug('response from server: "%s"', response)

    logger.debug('waiting for response')
    responsecaly = s.recv(MAX_LENGTH)
    logger.debug('response from server: "%s"', responsecaly)

    i = 0

    while responsecalx.split("\n")[1][0] != '1' and responsecaly.split("\n")[1][0] != '1' and i < 15:
        i+=1
        message = 'COM8 calibrate x 39.735'
        logger.debug('sending data: "%s"', message)
        s.send(message)
        time.sleep(10)

        # Receive a response
        logger.debug('waiting for response')
        response = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', response)

        logger.debug('waiting for response')
        response = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', response)

        message = 'COM8 calibrated x'
        logger.debug('sending data: "%s"', message)
        s.send(message)

        # Receive a response
        logger.debug('waiting for response')
        response = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', response)

        logger.debug('waiting for response')
        responsecalx = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', responsecalx)

        message = 'COM8 calibrate y 10.5'
        logger.debug('sending data: "%s"', message)
        s.send(message)
        time.sleep(10)

        # Receive a response
        logger.debug('waiting for response')
        response = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', response)

        logger.debug('waiting for response')
        response = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', response)

        message = 'COM8 calibrated y'
        logger.debug('sending data: "%s"', message)
        s.send(message)

        # Receive a response
        logger.debug('waiting for response')
        response = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', response)

        logger.debug('waiting for response')
        responsecaly = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', responsecaly)


    message = 'COM8 calibrated y'
    logger.debug('sending data: "%s"', message)
    s.send(message)

    # Receive a response
    logger.debug('waiting for response')
    response = s.recv(MAX_LENGTH)
    logger.debug('response from server: "%s"', response)

    logger.debug('waiting for response')
    responsecal = s.recv(MAX_LENGTH)
    logger.debug('response from server: "%s"', responsecal)

    i = 0

    while responsecal.split("\n")[1][0] != '1' and i < 15:
        i+=1
        message = 'COM8 calibrate y 10.5'
        logger.debug('sending data: "%s"', message)
        s.send(message)
        time.sleep(10)

        # Receive a response
        logger.debug('waiting for response')
        response = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', response)

        logger.debug('waiting for response')
        response = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', response)

        message = 'COM8 calibrated y'
        logger.debug('sending data: "%s"', message)
        s.send(message)

        # Receive a response
        logger.debug('waiting for response')
        response = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', response)

        logger.debug('waiting for response')
        responsecal = s.recv(MAX_LENGTH)
        logger.debug('response from server: "%s"', responsecal)



    # # Home x
    # message = 'COM8 calibrate x 38.735'
    # logger.debug('sending data: "%s"', message)
    # s.send(message)
    #
    # # Receive a response
    # logger.debug('waiting for response')
    # response = s.recv(MAX_LENGTH)
    # logger.debug('response from server: "%s"', response)
    #
    # # Home y
    # message = 'COM8 calibrate y 10.5'
    # logger.debug('sending data: "%s"', message)
    # s.send(message)
    # time.sleep(1)
    #
    # # Receive a response
    # logger.debug('waiting for response')
    # response = s.recv(MAX_LENGTH)
    # logger.debug('response from server: "%s"', response)
    #
    # # Home x
    # message = 'COM8 home x'
    # logger.debug('sending data: "%s"', message)
    # s.send(message)
    # time.sleep(1)
    #
    # # Receive a response
    # logger.debug('waiting for response')
    # response = s.recv(MAX_LENGTH)
    # logger.debug('response from server: "%s"', response)
    #
    # # Home y
    # message = 'COM8 home y'
    # logger.debug('sending data: "%s"', message)
    # s.send(message)
    # time.sleep(1)
    #
    # # Receive a response
    # logger.debug('waiting for response')
    # response = s.recv(MAX_LENGTH)
    # logger.debug('response from server: "%s"', response)
    #
    # # Send x to the middle
    # message = 'COM8 distance x %f' % (38.735/2.0)
    # logger.debug('sending data: "%s"', message)
    # s.send(message)
    # time.sleep(1)
    #
    # # Receive a response
    # logger.debug('waiting for response')
    # response = s.recv(MAX_LENGTH)
    # logger.debug('response from server: "%s"', response)
    #
    # # Send y to the middle
    # message = 'COM8 distance y %f' % (10.5/2.0)
    # logger.debug('sending data: "%s"', message)
    # s.send(message)
    # time.sleep(1)
    #
    # # Receive a response
    # logger.debug('waiting for response')
    # response = s.recv(MAX_LENGTH)
    # logger.debug('response from server: "%s"', response)
    #
    # # Move z around some
    # message = 'COM8 debug z -500'
    # logger.debug('sending data: "%s"', message)
    # s.send(message)
    # time.sleep(1)
    #
    # # Receive a response
    # logger.debug('waiting for response')
    # response = s.recv(MAX_LENGTH)
    # logger.debug('response from server: "%s"', response)
    #
    # # Home x
    # message = 'COM8 home x'
    # logger.debug('sending data: "%s"', message)
    # s.send(message)
    # time.sleep(1)
    #
    # # Receive a response
    # logger.debug('waiting for response')
    # response = s.recv(MAX_LENGTH)
    # logger.debug('response from server: "%s"', response)
    #
    # # Home y
    # message = 'COM8 home y'
    # logger.debug('sending data: "%s"', message)
    # s.send(message)
    # time.sleep(1)
    #
    # # Receive a response
    # logger.debug('waiting for response')
    # response = s.recv(MAX_LENGTH)
    # logger.debug('response from server: "%s"', response)

    # Clean up
    logger.debug('closing socket')
    s.close()
    logger.debug('done')

if __name__ == '__main__':
    main()
