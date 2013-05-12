u"""A threaded version of the client (hopefully) supporting the same protocol as
the server."""

import readline, socket, sys, threading

# Yes, this is the ugly way to do it.
# TODO: Make this more beautiful
HOST = sys.argv[1]
PORT = int(sys.argv[2])

RECV_SIZE = 4096

def recieve_and_print(s):
    u"""Recieves and prints, loops!"""
    while True:
        data = s.recv(RECV_SIZE)
        if not data:
            break
        print 'got>', data

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    threading.Thread(target=recieve_and_print, name=u'Receiver thread', args=[s]).start()

    while True:
        try:
            user_input = raw_input('send> ')
        except EOFError:
            break

        s.send(user_input)

finally:
    s.close()


