u"""A naive version of the client (hopefully) supporting the same protocol as
the server."""

import readline, socket, sys

# Yes, this is the ugly way to do it.
# TODO: Make this more beautiful
HOST = sys.argv[1]
PORT = int(sys.argv[2])

RECV_SIZE = 4096

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    while True:
        try:
            user_input = raw_input('send> ')
        except EOFError:
            break

        s.send(user_input)

        data = s.recv(RECV_SIZE)
        print 'got>', data

finally:
    s.close()

