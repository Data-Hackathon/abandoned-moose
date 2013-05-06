u"""This is the naive implementation of a server handling many concurrent
connections at once."""

import itertools, socket

HOST = '127.0.0.1'
PORT = 9001

def reverse(data):
    return ''.join(reversed(data))

commands = {'reverse' : reverse }

def dispatcher(data):
    u"""Receives data, parses command and the payload. Then runs the command
    with the payload."""

    parts = data.split(';', 1)

    cmd = commands.get(parts[0], False)
    if cmd and len(parts) > 1:
        return cmd(parts[1])
    else:
        return data

# TODO: Implement this, you know you want to! :)
# def broadcast(*args, **kwargs):
#     u"""Receives a message and sends it to all connected clients."""
# 
#     pass

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))

print u'Starting and connecting to host: {0} and port {1}'.format(HOST, PORT)

s.listen(1) # Activate the server

print u'Waiting for connections...'

conn, addr = s.accept()

try:
    print u'Connected by addr: {0}'.format(addr)

    while True:
        print u'Receiving...'
        data = conn.recv(1024)
        if not data:
            break

        print u'Got data:'
        print data
        print u'Processing and sending...'
        conn.send(dispatcher(data))

finally:
    conn.close()
