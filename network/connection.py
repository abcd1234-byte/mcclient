import socket
from time import time
from select import select

import network.messages as messages


class Connection(object):
    def __init__(self, name):
        self.socket = None
        self.name = name


    def connect(self, host, port=25565):
        self.socket = socket.create_connection((host, port))
        handshake = messages.Handshake(self.name)
        handshake.send(self.socket)
        answer = messages.get(self.socket)
        assert answer.pseudo == '-'
        login = messages.Login(11, self.name)
        login.send(self.socket)
        answer = messages.get(self.socket)
        print('Dimension: %d' % answer.dimension)


    def get_messages(self):
        pending_messages = []
        inputs, outputs, errors = select([self.socket], [], [], 0)
        while inputs:
            message = messages.get(self.socket)
            pending_messages.append(message)
            if message == messages.Disconnect:
                break
            inputs, outputs, errors = select(inputs, [], [], 0)
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

