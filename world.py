import zlib
from collections import namedtuple
import struct
from blocks import Block, FaceDirections, FaceOrientations

#Block = namedtuple('Block', ['type', 'metadata', 'blocklight', 'skylight'])
#Blocktype = namedtuple('Blocktype', ['id', 'transparency', 'textures', ...

class Sector(object):
    def __init__(self, cx, cz):
        self.cx, self.cz = cx, cz
        self.blocks = [[[0] * 128 for y in xrange(16)] for x in xrange(16)]
        self.metadatas = [[[0] * 128 for y in xrange(16)] for x in xrange(16)]
        self.lighting = [[[0] * 128 for y in xrange(16)] for x in xrange(16)]
        self.faces = []
        self.alpha_faces = []
        self.fully_loaded = False
        self.changed = False #TODO


    def set_block(self, x, y, z, type, metadata=None, blocklight=None, skylight=None):
        #TODO
        self.blocks[x][z][y] = type #TODO
        if metadata is not None:
            self.metadatas[x][z][y] = metadata
        #TODO: optimize
        self.changed = True #TODO


    def get_block_type(self, x, y, z):
        return self.blocks[x][z][y]


    def get_block_data(self, x, y, z):
        return self.metadatas[x][z][y]


    def get_block_lighting(self, x, y, z):
        return self.lighting[x][z][y]


    def set_chunk(self, ox, oy, oz, size_x, size_y, size_z, data):
        #TODO: optimize
        if size_x == 16 and size_y == 128 and size_z == 16:
            self.fully_loaded = True #TODO
        for x in xrange(size_x):
            for y in xrange(size_y):
                for z in xrange(size_z):
                    index = y + (z * size_y) + (x * size_y * size_z)
                    self.blocks[ox + x][oz + z][oy + y] = ord(data[index])
                    metadata = ord(data[size_x * size_y * size_z + (index // 2)])
                    lighting = ord(data[size_x * size_y * size_z * 3 // 2 + (index // 2)])
                    if index % 2 == 1:
                        self.lighting[ox + x][oz + z][oy + y] = lighting >> 4
                        self.metadatas[ox + x][oz + z][oy + y] = metadata >> 4
                    else:
                        self.lighting[ox + x][oz + z][oy + y] = lighting & 15
                        self.metadatas[ox + x][oz + z][oy + y] = metadata & 15
        self.changed = True #TODO


    def _gen_faces(self):
        if not self.fully_loaded:
            return

        print('Gen faces (%d ; %d)' % (self.cx, self.cz))

        xb = self.cx * 16
        zb = self.cz * 16

        #TODO: optimize and cleanup

        self.faces = []
        self.alpha_faces = []
        for x, blocks in enumerate(self.blocks):
            for z, blocks in enumerate(blocks):
                for y, block in enumerate(blocks):
                    pos = x + xb, y, z + zb
                    if block != 0 and not Block.from_type(block).transparent:
                        if x == 15 or not self.blocks[x + 1][z][y] or Block.from_type(self.blocks[x + 1][z][y]).transparent:
                            self.faces.append((pos, FaceDirections.SOUTH))
                        if y == 127 or not self.blocks[x][z][y + 1] or Block.from_type(self.blocks[x][z][y + 1]).transparent:
                            self.faces.append((pos, FaceDirections.TOP))
                        if z == 15 or not self.blocks[x][z + 1][y] or Block.from_type(self.blocks[x][z + 1][y]).transparent:
                            self.faces.append((pos, FaceDirections.WEST))
                        if x == 0 or not self.blocks[x-1][z][y] or Block.from_type(self.blocks[x - 1][z][y]).transparent:
                            self.faces.append((pos, FaceDirections.NORTH))
                        if y == 0 or not self.blocks[x][z][y-1] or Block.from_type(self.blocks[x][z][y - 1]).transparent:
                            self.faces.append((pos, FaceDirections.BOTTOM))
                        if z == 0 or not self.blocks[x][z-1][y] or Block.from_type(self.blocks[x][z - 1][y]).transparent:
                            self.faces.append((pos, FaceDirections.EAST))
                    elif block != 0 and Block.from_type(block).transparent:
                        if x == 15 or not self.blocks[x + 1][z][y] or Block.from_type(self.blocks[x + 1][z][y]).transparent:
                            self.alpha_faces.append((pos, FaceDirections.SOUTH))
                        if y == 127 or not self.blocks[x][z][y + 1] or Block.from_type(self.blocks[x][z][y + 1]).transparent:
                            self.alpha_faces.append((pos, FaceDirections.TOP))
                        if z == 15 or not self.blocks[x][z + 1][y] or Block.from_type(self.blocks[x][z + 1][y]).transparent:
                            self.alpha_faces.append((pos, FaceDirections.WEST))
                        if x == 0 or not self.blocks[x-1][z][y] or Block.from_type(self.blocks[x - 1][z][y]).transparent:
                            self.alpha_faces.append((pos, FaceDirections.NORTH))
                        if y == 0 or not self.blocks[x][z][y-1] or Block.from_type(self.blocks[x][z][y - 1]).transparent:
                            self.alpha_faces.append((pos, FaceDirections.BOTTOM))
                        if z == 0 or not self.blocks[x][z-1][y] or Block.from_type(self.blocks[x][z - 1][y]).transparent:
                            self.alpha_faces.append((pos, FaceDirections.EAST))


    def get_faces(self):
        if self.changed:
            self.changed = False
            self._gen_faces()
        return self.faces, self.alpha_faces



class World(object):
    def __init__(self):
        # Sector: sectors[(x, z)]
        self.sectors = {}
        self.faces = []
        self.changed = False
        self.central_sector = (0, 0)

    def delete_sector(self, cx, cz):
        del self.sectors[(cx, cz)]
        self.changed = True


    def get_block_coords(self, x, y, z):
        return (x // 16, z // 16, x % 16, y, z % 16)


    def allocate_sector(self, cx, cz):
        # [x][z][y]
        self.sectors[(cx, cz)] = Sector(cx, cz)


    def map_chunk(self, x, y, z, size_x, size_y, size_z, compressed_data):
        #TODO
        cx, cz, ox, oy, oz = self.get_block_coords(x, y, z)
        assert ox + size_x <= 16
        assert oy + size_y <= 128
        assert oz + size_z <= 16

        data = zlib.decompress(compressed_data)
        self.sectors[cx, cz].set_chunk(ox, oy, oz, size_x, size_y, size_z, data)
        size = 2
        if abs(cx - self.central_sector[0]) <= size and abs(cz - self.central_sector[1]) <= size:
            self.changed = True


    def modify_block(self, x, y, z, type, metadata):
        cx, cz, ox, oy, oz = self.get_block_coords(x, y, z)
        self.sectors[cx, cz].set_block(ox, oy, oz, type, metadata)
        self.changed = True


    def regen_faces(self, size):
        self.faces = []
        alpha_faces = []
        print('Regen world faces')
        for (cx, cz), sector in self.sectors.iteritems():
            if abs(cx - self.central_sector[0]) <= size and abs(cz - self.central_sector[1]) <= size:
                opaque, alpha = sector.get_faces()
                self.faces.extend(opaque)
                alpha_faces.extend(alpha)

        #TODO: sort alpha faces
        self.faces.extend(alpha_faces)

        self.changed = False
        print('Regen ended.')


    def needs_updating(self, obs_x, obs_y, obs_z):
        return self.changed or self.central_sector != (int(obs_x) // 16, int(obs_z) // 16)


    def get_gl_faces(self, obs_x, obs_y, obs_z):
        self.central_sector = int(obs_x) // 16, int(obs_z) // 16
        self.regen_faces(2)

        vertex = []
        texcoords = []
        colors = []
        types = set()
        for (x, y, z), face in self.faces:
            nx, ny, nz = FaceDirections.directions[face]
            if abs(x - obs_x) > 20 or abs(y - obs_y) > 20 or abs(z - obs_z) > 20:
                continue

            #TODO: backface culling
            #if nx * (x - obs_x) + ny * (y - obs_y) + nz * (z - obs_z) >= 0:
            #    continue

            cx, cz, ox, oy, oz = self.get_block_coords(x, y, z)
            cx2, cz2, ox2, oy2, oz2 = self.get_block_coords(x + nx, y + ny, z + nz)
            type = self.sectors[cx, cz].get_block_type(ox, oy, oz)
            lighting = self.sectors[cx2, cz2].get_block_lighting(ox2, oy2, oz2)
            types.add(type)
            u, v, orientation = Block.from_type(type).get_face_texture(self.sectors[cx, cz].get_block_data(ox, oy, oz), face)
            u, v = u / 16., 15. / 16. - v / 16.

            maps = FaceOrientations.uvmaps[orientation]
            texcoords.extend((u2 / 16. + u, v2 / 16. + v) for u2, v2 in maps)

            colors.extend([(0.8 ** (15 - lighting),
                            0.8 ** (15 - lighting),
                            0.8 ** (15 - lighting))] * 4)

            #TODO: simplifier
            if nx == 1:
                vertex.extend([(x + 1, y,     z),
                               (x + 1, y + 1, z),
                               (x + 1, y + 1, z + 1),
                               (x + 1, y,     z + 1)])
            elif nx == -1:
                vertex.extend([(x, y,     z),
                               (x, y + 1, z),
                               (x, y + 1, z + 1),
                               (x, y,     z + 1)])
            elif ny == 1:
                vertex.extend([(x,     y + 1, z),
                               (x,     y + 1, z + 1),
                               (x + 1, y + 1, z + 1),
                               (x + 1, y + 1, z)])
            elif ny == -1:
                vertex.extend([(x,     y, z),
                               (x,     y, z + 1),
                               (x + 1, y, z + 1),
                               (x + 1, y, z)])
            elif nz == 1:
                vertex.extend([(x,     y,     z + 1),
                               (x,     y + 1, z + 1),
                               (x + 1, y + 1, z + 1),
                               (x + 1, y,     z + 1)])
            elif nz == -1:
                vertex.extend([(x,     y,     z),
                               (x,     y + 1, z),
                               (x + 1, y + 1, z),
                               (x + 1, y,     z)])
        print('Encountered types: %r' % types)
        return vertex, texcoords, colors

