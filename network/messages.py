import struct

__all__ = ['get', 'Handshake', 'Login', 'SpawnPosition']

_known_messages = {}


def register(cls):
    _known_messages[cls.id] = cls
    return cls


def get(socket):
    id, = struct.unpack('!B', socket.recv(1))
    return _known_messages[id].get(socket)



def read_metadata(socket):
    data = []
    x, = struct.unpack('!B', socket.recv(1))
    while x != 127:
        type = ['b', 'h', 'i', 'f', str, ][x >> 5] #TODO
        if type is str:
            data.append(read_string(socket))
        else:
            data.append(struct.unpack('!%s' % type, socket.recv(struct.calcsize(type))))

        x, = struct.unpack('!B', socket.recv(1))
    return data



def write_string(socket, string):
    data = string.encode('utf-16-be')
    data = struct.pack('!H%ds' % len(data), len(string), data)
    socket.send(data)


def read_string(socket):
    length, = struct.unpack('!H', socket.recv(2))
    return socket.recv(length * 2).decode('utf-16-be')



@register
class KeepAlive(object):
    id = 0x00

    def __init__(self):
        pass

    @classmethod
    def get(cls, socket):
        return cls()

    def send(self, socket):
        socket.send(struct.pack('!B', self.id))



@register
class Handshake(object):
    id = 0x02

    def __init__(self, pseudo):
        self.pseudo = pseudo


    @classmethod
    def get(cls, socket):
        return cls(read_string(socket))


    def send(self, socket):
        socket.send(struct.pack('!B', self.id))
        write_string(socket, self.pseudo)


@register
class ChatMessage(object):
    id = 0x03

    def __init__(self, message):
        self.message = message

    @classmethod
    def get(cls, socket):
        return cls(read_string(socket))

    def send(self, socket):
        socket.send(struct.pack('!B', self.id))
        write_string(socket, self.message)


@register
class Login(object):
    id = 0x01

    def __init__(self, proto_version, pseudo, seed=0, dimension=0):
        self.proto_version = proto_version
        self.pseudo = pseudo
        self.seed = seed
        self.dimension = dimension


    @classmethod
    def get(cls, socket):
        return cls(struct.unpack('!I', socket.recv(4))[0], read_string(socket),
                   struct.unpack('!Q', socket.recv(8))[0], struct.unpack('!B', socket.recv(1)[0]))


    def send(self, socket):
        socket.send(struct.pack('!BI', self.id, self.proto_version))
        write_string(socket, self.pseudo)
        socket.send(struct.pack('!QB', self.seed, self.dimension))


@register
class TimeUpdate(object):
    id = 0x04

    def __init__(self, time):
        self.time = time


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!Q', socket.recv(8)))


@register
class EntityEquipment(object):
    id = 0x05

    def __init__(self, eid, slot, item_id, todo):
        self.eid = eid
        self.slot = slot
        self.item_id = item_id
        self.todo = todo #TODO


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!IHHh', socket.recv(10)))



@register
class SpawnPosition(object):
    id = 0x06

    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!iii', socket.recv(4 * 3)))


@register
class UpdateHealth(object):
    id = 0x08

    def __init__(self, health):
        self.health = health

    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!H', socket.recv(2)))


@register
class Animation(object):
    id = 0x12

    def __init__(self, eid, animation):
        self.eid = eid
        self.animation = animation


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!IB', socket.recv(5)))

    def send(self, socket):
        socket.write(struct.pack('!BIB', self.id, self.eid, self.animation))



@register
class NamedEntitySpawn(object):
    id = 0x14

    def __init__(self, eid, name, x, y, z, rotation, pitch, item):
        self.eid = eid
        self.x, self.y, self.z = x, y, z
        self.rotation = rotation
        self.pitch = pitch
        self.item = item


    @classmethod
    def get(cls, socket):
        args = list(struct.unpack('!I', socket.recv(4)))
        args.append(read_string(socket))
        args.extend(struct.unpack('!iiibbH', socket.recv(16)))
        return cls(*args)


@register
class PickupSpawn(object):
    id = 0x15

    def __init__(self, eid, item_id, count, data, x, y, z, rotation, pitch, roll):
        self.eid = eid
        self.item_id = item_id
        self.count = count
        self.data = data
        self.x, self.y, self.z = x, y, z
        self.rotation = rotation
        self.pitch = pitch
        self.roll = roll


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!IhBhiiibbb', socket.recv(24)))



@register
class MobSpawn(object):
    id = 0x18

    def __init__(self, eid, etype, x, y, z, yaw, pitch, data):
        self.eid = eid
        self.etype = etype
        self.x, self.y, self.z = x, y, z
        self.yaw = yaw
        self.pitch = pitch
        self.data = data


    @classmethod
    def get(cls, socket):
        args = list(struct.unpack('!IBiiibb', socket.recv(19)))
        args.append(read_metadata(socket))
        return cls(*args)


@register
class EntityVelocity(object):
    id = 0x1c

    def __init__(self, eid, vx, vy, vz):
        self.eid = eid
        self.vx, self.vy, self.vz = vx, vy, vz


    @classmethod
    def get(cls, socket):
        args = list(struct.unpack('!Ihhh', socket.recv(10)))
        return cls(*args)



@register
class DestroyEntity(object):
    id = 0x1d

    def __init__(self, eid):
        self.eid = eid


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!I', socket.recv(4)))


@register
class EntityStatus(object):
    id = 0x26

    def __init__(self, eid, status):
        self.eid = eid
        self.status = status


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!Ib', socket.recv(5)))



@register
class EntityMetaData(object):
    id = 0x28

    def __init__(self, eid, metadata):
        self.eid = eid
        self.metadata = metadata

    @classmethod
    def get(cls, socket):
        args = list(struct.unpack('!I', socket.recv(4)))
        args.append(read_metadata(socket))
        return cls(*args)



@register
class PreChunk(object):
    id = 0x32

    def __init__(self, x, z, mode):
        self.x, self.z = x, z
        self.mode = mode


    @classmethod
    def get(cls, socket):
        args = struct.unpack('!ii?', socket.recv(9))
        return cls(*args)


@register
class MapChunk(object):
    id = 0x33 #TODO: check

    def __init__(self, x, y, z, size_x, size_y, size_z, compressed_size, compressed_data):
        self.x, self.y, self.z = x, y, z
        self.size_x, self.size_y, self.size_z = size_x, size_y, size_z
        self.compressed_size = compressed_size
        self.compressed_data = compressed_data

    @classmethod
    def get(cls, socket):
        x, y, z, size_x, size_y, size_z, compressed_size = list(struct.unpack('!ihiBBBI', socket.recv(17)))
        compressed_data = socket.recv(compressed_size)
        size_x, size_y, size_z = size_x + 1, size_y + 1, size_z + 1
        return cls(x, y, z, size_x, size_y, size_z, compressed_size, compressed_data)


@register
class MultiBlockChange(object):
    id = 0x34 #TODO: check

    def __init__(self, chunk_x, chunk_z, array_size, coordinates, types, metadatas):
        self.chunk_x, self.chunk_z = chunk_x, chunk_z
        self.array_size = array_size
        self.coordinates = coordinates
        self.types = types
        self.metadatas = metadatas


    @classmethod
    def get(cls, socket):
        chunk_x, chunk_y, array_size = struct.unpack('!iiH', socket.recv(10))
        coordinates = [(b >> 12, (b >> 8 & 0xF), b & 0xFF)
                            for b in struct.unpack('!%s' % ('H' * array_size),
                                                   socket.recv(2 * array_size))]
        types = struct.unpack('!%s' % ('B' * array_size), socket.recv(array_size))
        metadatas = struct.unpack('!%s' % ('B' * array_size), socket.recv(array_size))
        return cls(chunk_x, chunk_y, array_size, coordinates, types, metadatas)



@register
class BlockChange(object):
    id = 0x35

    def __init__(self, x, y, z, block_type, block_metadata):
        self.x, self.y, self.z = x, y, z
        self.block_type = block_type
        self.block_metadata = block_metadata


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!ibiBB', socket.recv(11)))



@register
class PlayerPosLook(object):
    id = 0x0d

    def __init__(self, x, y, stance, z, yaw, pitch, on_ground):
        self.x, self.y, self.z = x, y, z
        self.stance = stance
        self.yaw = yaw
        self.pitch = pitch
        self.on_ground = on_ground


    @classmethod
    def get(cls, socket):
        x, stance, y, z, yaw, pitch, on_ground = struct.unpack('!ddddff?', socket.recv(41))
        return cls(x, y, stance, z, yaw, pitch, on_ground)

    def send(self, socket):
        socket.send(struct.pack('!Bddddff?', self.id, self.x, self.y, self.stance,
                                            self.z, self.yaw, self.pitch,
                                            self.on_ground))


@register
class CollectItem(object):
    id = 0x16

    def __init__(self, collector_eid, collected_eid):
        self.collector_eid = collector_eid
        self.collected_eid = collected_eid

    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!II', socket.recv(8)))



@register
class AddObject(object):
    id = 0x17

    def __init__(self, eid, type, x, y, z):
        self.eid = eid
        self.type = type
        self.x, self.y, self.z = x, y, z


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!IBiii', socket.recv(17)))


@register
class Painting(object):
    id = 0x19

    def __init__(self, eid, title, x, y, z, direction):
        self.eid = eid
        self.title = title
        self.x, self.y, self.z = x, y, z
        self.direction = direction

    @classmethod
    def get(cls, socket):
        args = list(struct.unpack('!I', socket.recv(4)))
        args.append(read_string(socket))
        args.extend(struct.unpack('!iiii', socket.recv(16)))
        return cls(*args)



@register
class EntityRelativeMove(object):
    id = 0x1f

    def __init__(self, eid, dx, dy, dz):
        self.eid = eid
        self.dx, self.dy, self.dz = dx, dy, dz


    @classmethod
    def get(cls, socket):
        args = struct.unpack('!Ibbb', socket.recv(7))
        return cls(*args)


@register
class EntityLookRelativeMove(object):
    id = 0x21

    def __init__(self, eid, dx, dy, dz, yaw, pitch):
        self.eid = eid
        self.dx, self.dy, self.dz = dx, dy, dz
        self.yaw = yaw
        self.pitch = pitch


    @classmethod
    def get(cls, socket):
        args = struct.unpack('!Ibbbbb', socket.recv(9))
        return cls(*args)


@register
class EntityTeleport(object):
    id = 0x22

    def __init__(self, eid, x, y, z, yaw, pitch):
        self.eid = eid
        self.x, self.y, self.z = x, y, z
        self.yaw = yaw
        self.pitch = pitch

    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!Iiiibb', socket.recv(18)))



@register
class Explosion(object):
    id = 0x3c

    def __init__(self, x, y, z, unknown, count, unknown_data):
        self.x, self.y, self.z = x, y, z
        self.unknown = unknown
        self.count = count
        self.unknown_data = unknown_data


    @classmethod
    def get(cls, socket):
        x, y, z, unknown, count = struct.unpack('!dddfI', socket.recv(32))
        return cls(x, y, z, unknown, count, socket.recv(3 * count))



@register
class NewInvalidState(object):
    id = 0x46

    def __init__(self, reason):
        self.reason = reason

    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!B', socket.recv(1)))


@register
class Weather(object):
    id = 0x47 #TODO

    def __init__(self, eid, unkwn1, unkwn2, unkwn3, unkwn4):
        self.eid = eid
        self.unkwn1, self.unkwn2, self.unkwn3, self.unkwn4 = unkwn1, unkwn2, unkwn3, unkwn4


    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!I?iii', socket.recv(17)))



@register
class SetSlot(object):
    id = 0x67
    #TODO

    def __init__(self, window_id, slot, item):
        self.window_id = window_id
        self.slot = slot
        self.item = item


    @classmethod
    def get(cls, socket):
        window_id, slot, item_id = struct.unpack('!BHh', socket.recv(5))
        if item_id != -1:
            item = [item_id]
            item.extend(struct.unpack('!BH', socket.recv(3)))
        else:
            item = None
        return cls(window_id, slot, item)


@register
class WindowItems(object):
    id = 0x68
    #TODO

    def __init__(self, id, count, payload):
        self.id = id
        self.count = count
        self.payload = payload


    @classmethod
    def get(cls, socket):
        args = list(struct.unpack('!BH', socket.recv(3)))
        count = args[-1]
        payload = []
        for slot in range(count):
            item_id, = struct.unpack('!h', socket.recv(2))
            if item_id == -1:
                payload.append(None)
            else:
                item = [item_id]
                item.extend(struct.unpack('!BH', socket.recv(3)))
                payload.append(tuple(item))
        args.append(payload)
        return cls(*args)


@register
class UpdateSign(object):
    id = 0x82

    def __init__(self, x, y, z, text1, text2, text3, text4):
        self.x, self.y, self.z = x, y, z
        self.text1 = text1
        self.text2 = text2
        self.text3 = text3
        self.text4 = text4


    @classmethod
    def get(cls, socket):
        args = list(struct.unpack('!ihi', socket.recv(10)))
        args.append(read_string(socket))
        args.append(read_string(socket))
        args.append(read_string(socket))
        args.append(read_string(socket))
        return cls(*args)


@register
class IncrementStatistic(object):
    id = 0xc8

    def __init__(self, statistic_id, amount):
        self.statistic_id = statistic_id
        self.amount = amount

    @classmethod
    def get(cls, socket):
        return cls(*struct.unpack('!Ib', socket.recv(5)))



@register
class Disconnect(object):
    id = 0xff

    def __init__(self, reason='byebye'):
        self.reason = reason


    @classmethod
    def get(cls, socket):
        return cls(read_string(socket))

    def send(self, socket):
        socket.send(struct.pack('!B', self.id))
        write_string(socket, self.reason)

