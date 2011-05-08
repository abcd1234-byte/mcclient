import socket
from time import time
from select import select
from .buffer import Buffer, BufferReader, BufferException

import network.messages as messages


class Connection(object):
    def __init__(self, name):
        self.socket = None
        self._buffer = Buffer()
        self.name = name


    def connect(self, host, port=25565):
        self.socket = socket.create_connection((host, port))
        handshake = messages.Handshake(self.name)
        handshake.send(self.socket)
        self._buffer.write(self.socket.recv(1024))
        answer = messages.get(BufferReader(self._buffer))
        assert answer.pseudo == '-'
        login = messages.Login(11, self.name)
        login.send(self.socket)
        self._buffer.write(self.socket.recv(1024))
        answer = messages.get(BufferReader(self._buffer))
        print('Dimension: %d' % answer.dimension)


    def get_messages(self):
        pending_messages = []
        inputs, outputs, errors = select([self.socket], [], [], 0)
        while inputs:
            self._buffer.write(self.socket.recv(1024))
            inputs, outputs, errors = select(inputs, [], [], 0)

        try:
            while True:
                message = messages.get(BufferReader(self._buffer))
                pending_messages.append(message)
        except BufferException:
            pass

        return pending_messages


if __name__ == '__main__':
    truc  = Connection('Bot')
    truc.connect('localhost')
    messages.KeepAlive().send(truc.socket)

    last_ping = 0

    while truc.socket:
        message = messages.get(truc.socket)

        if time() - last_ping > 2:
            messages.KeepAlive().send(truc.socket)
            last_ping = time()

        if type(message) == messages.ChatMessage:
            print(message)
            print(message.message)

