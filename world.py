import zlib
from blocks import get_block_texture
from sector import Sector as CSector
from time import time
from worldrenderer import WorldRenderer



class World(object):
    def __init__(self):
        # Sector: sectors[(x, z)]
        self.csectors = {}
        self.faces = []
        self.renderer = WorldRenderer(self.csectors, get_block_texture)


    def delete_sector(self, cx, cz):
        del self.csectors[(cx, cz)]


    def get_block_coords(self, x, y, z):
        return (x // 16, z // 16, x % 16, y, z % 16)


    def allocate_sector(self, cx, cz):
        # [x][z][y]
        self.csectors[(cx, cz)] = CSector(cx, cz)
        for dx, dz in ((-1, 0), (1, 0), (0, -1), (0, 1)):
            key = cx + dx, cz + dz
            if key in self.csectors:
                self.csectors[(cx, cz)].add_neighbour(self.csectors[key], (dx, dz))


    def map_chunk(self, x, y, z, size_x, size_y, size_z, compressed_data):
        cx, cz, ox, oy, oz = self.get_block_coords(x, y, z)
        assert ox + size_x <= 16
        assert oy + size_y <= 128
        assert oz + size_z <= 16

        data = zlib.decompress(compressed_data)
        if (cx, cz) not in self.csectors:
            self.allocate_sector(cx, cz)
        self.csectors[cx, cz].set_chunk(ox, oy, oz, size_x, size_y, size_z, data)


    def modify_block(self, x, y, z, type, metadata):
        cx, cz, ox, oy, oz = self.get_block_coords(x, y, z)
        try:
            self.csectors[cx, cz].set_block(ox, oy, oz, type, metadata)
        except KeyError:
            pass


    def get_gl_faces(self, pos, fov, ratio, znear, zfar, yaw, pitch):
        renderer = self.renderer

        oldtime = time()
        self.renderer.render(pos, fov, ratio, znear, zfar, yaw, pitch)
        newtime = time()
        print('Time ellapsed: %f' % (newtime - oldtime))
        return renderer.nb_vertices, renderer.vertices, renderer.texcoords, renderer.colors

