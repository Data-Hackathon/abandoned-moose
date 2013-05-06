u"""A naive version of the client (hopefully) supporting the same protocol as
the server."""

import socket

HOST = 'localhost'
PORT = 9001

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    while True:
        try:
            user_input = raw_input('send> ')
        except EOFError:
            break

        s.send(user_input)

        data = s.recv(1024)
        print 'got>', data

finally:
    s.close()

