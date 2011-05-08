class BufferException:
    pass


class Buffer(object):
    def __init__(self):
        self._buffer = ""


    def read(self, size):
        if len(self._buffer) >= size:
            data = self._buffer[:size]
            self._buffer = self._buffer[size:]
            return data
        else:
            raise BufferException


    def write(self, data):
        self._buffer += data


    def prepend(self, data):
        self._buffer = data + self._buffer



class BufferReader(object):
    def __init__(self, buffer):
        self.buffer = buffer
        self._read_data = []


    def read(self, size):
        try:
            data = self.buffer.read(size)
        except BufferException:
            self.buffer.prepend(b''.join(self._read_data))
            raise
        else:
            self._read_data.append(data)
            return data

