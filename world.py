import zlib
from collections import namedtuple
import struct
from blocks import Block, FaceDirections, FaceOrientations, get_block_texture
from sector import Sector as CSector
from time import time
from worldrenderer import WorldRenderer
#Block = namedtuple('Block', ['type', 'metadata', 'blocklight', 'skylight'])
#Blocktype = namedtuple('Blocktype', ['id', 'transparency', 'textures', ...



class World(object):
    def __init__(self):
        # Sector: sectors[(x, z)]
        self.csectors = {}
        self.faces = []
        self.changed = False
        self.central_sector = (0, 0)

    def delete_sector(self, cx, cz):
        del self.csectors[(cx, cz)]
        self.changed = True


    def get_block_coords(self, x, y, z):
        return (x // 16, z // 16, x % 16, y, z % 16)


    def allocate_sector(self, cx, cz):
        # [x][z][y]
        self.csectors[(cx, cz)] = CSector(cx, cz)


    def map_chunk(self, x, y, z, size_x, size_y, size_z, compressed_data):
        #TODO
        cx, cz, ox, oy, oz = self.get_block_coords(x, y, z)
        assert ox + size_x <= 16
        assert oy + size_y <= 128
        assert oz + size_z <= 16

        data = zlib.decompress(compressed_data)
        self.csectors[cx, cz].set_chunk(ox, oy, oz, size_x, size_y, size_z, data)
        size = 2
        if abs(cx - self.central_sector[0]) <= size and abs(cz - self.central_sector[1]) <= size:
            self.changed = True


    def modify_block(self, x, y, z, type, metadata):
        cx, cz, ox, oy, oz = self.get_block_coords(x, y, z)
        self.sectors[cx, cz].set_block(ox, oy, oz, type, metadata)
        self.changed = True


    def needs_updating(self, obs_x, obs_y, obs_z):
        return self.changed or self.central_sector != (int(obs_x) // 16, int(obs_z) // 16)


    def get_gl_faces(self, obs_x, obs_y, obs_z):
        self.central_sector = int(obs_x) // 16, int(obs_z) // 16

        oldtime = time()
        wr = WorldRenderer(self.csectors, get_block_texture)
        wr.render((obs_x, obs_y, obs_z), 20)
        newtime = time()
        print('Time ellapsed: %f' % (newtime - oldtime))
        return wr.nb_vertices, wr.vertices, wr.texcoords, wr.colors

