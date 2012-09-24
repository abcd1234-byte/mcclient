# -*- encoding: utf-8 -*-
##
## Copyright (C) 2011 Thibaut GIRKA <thib@sitedethib.com>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation; version 3 only.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##


import zlib
from time import time
from cmcclient import WorldRenderer, Sector as CSector
from math import ceil, floor


def _make_aabb(cx, cz, ox, oy, oz):
    min_x = cx * 16 + ox
    min_y = oy
    min_z = cz * 16 + oz
    return (min_x, min_y, min_z, min_x + 1, min_y + 1, min_z + 1)


def _are_aabbs_colliding(aabb1, aabb2):
    a_min_x, a_min_y, a_min_z, a_max_x, a_max_y, a_max_z = aabb1
    b_min_x, b_min_y, b_min_z, b_max_x, b_max_y, b_max_z = aabb2
    xdeltaab = a_max_x - b_min_x
    xdeltaba = b_max_x - a_min_x
    ydeltaab = a_max_y - b_min_y
    ydeltaba = b_max_y - a_min_y
    zdeltaab = a_max_z - b_min_z
    zdeltaba = b_max_z - a_min_z
    if any(val < 0 for val in (xdeltaab, xdeltaba, ydeltaab, ydeltaba, zdeltaab, zdeltaba)):
        return None
    return min([(xdeltaab, (xdeltaab, 0, 0)),
                (xdeltaba, (-xdeltaba, 0, 0)),
                (ydeltaab, (0, ydeltaab, 0)),
                (ydeltaba, (0, -ydeltaba, 0)),
                (zdeltaab, (0, 0, zdeltaab)),
                (zdeltaba, (0, 0, -zdeltaba))])[1]


class World(object):
    def __init__(self):
        # Sector: sectors[(x, z)]
        self.csectors = {}
        self.faces = []
        self.renderer = WorldRenderer(self.csectors)


    def collide(self, message_pos):
        # bounding box seems to be around .27 (minimum) in all directions
        # We'll use .32 in all directions, for safety's sake

        # TODO: check for top

        hit_ceiling = False

        message_pos.on_ground = False
        under = set(self.get_block_coords(message_pos.x + dx, message_pos.y - .1, message_pos.z + dz)
                    for dx, dz in [(-.32, 0), (.32, 0), (0, 0), (0, -.32), (0, .32),
                                   (-.32, -.32), (-.32, .32), (.32, -.32), (.32, .32)])
        over = set(self.get_block_coords(message_pos.x + dx, message_pos.stance, message_pos.z + dz)
                    for dx, dz in [(-.32, 0), (.32, 0), (0, 0), (0, -.32), (0, .32)])

        neighbours = set()
        neighbours.update(self.get_block_coords(message_pos.x - .32, message_pos.y + dy, message_pos.z + dz)
                    for dy, dz in [(0, -.32), (0, .32), (1, .32), (1, -.32)])
        neighbours.update(self.get_block_coords(message_pos.x + .32, message_pos.y + dy, message_pos.z + dz)
                    for dy, dz in [(0, -.32), (0, .32), (1, .32), (1, -.32)])
        neighbours.update(self.get_block_coords(message_pos.x + dx, message_pos.y + dy, message_pos.z - .32)
                    for dy, dx in [(0, -.32), (0, .32), (1, .32), (1, -.32)])
        neighbours.update(self.get_block_coords(message_pos.x + dx, message_pos.y + dy, message_pos.z + .32)
                    for dy, dx in [(0, -.32), (0, .32), (1, .32), (1, -.32)])
        #TODO: check
        neighbours.update(self.get_block_coords(message_pos.x + dx, message_pos.stance + .2, message_pos.z + dz)
                    for dx, dz in [(-.32, 0), (.32, 0), (0, 0), (0, -.32), (0, .32),
                                   (-.32, -.32), (-.32, .32), (.32, -.32), (.32, .32)])

        try:
            under = set(_make_aabb(cx, cz, ox, oy, oz)
                    for cx, cz, ox, oy, oz in under
                    if self.csectors[cx, cz].get_block(ox, oy, oz).solid)
            neighbours = set(_make_aabb(cx, cz, ox, oy, oz)
                        for cx, cz, ox, oy, oz in neighbours
                        if self.csectors[cx, cz].get_block(ox, oy, oz).solid)
        except KeyError, IndexError:
            #TODO: the IndexError one is odd
            return False #TODO: explain why

        for aabb in under:
            player_aab = (message_pos.x - .32, message_pos.y, message_pos.z - .32,
                          message_pos.x + .32, message_pos.stance + .2, message_pos.z + .32)
            if _are_aabbs_colliding(aabb, player_aab):
                message_pos.on_ground = True
                diff = aabb[4] - message_pos.y
                message_pos.y += diff
                message_pos.stance += diff

        for aabb in neighbours:
            player_aab = (message_pos.x - .32, message_pos.y, message_pos.z - .32,
                          message_pos.x + .32, message_pos.stance + .2, message_pos.z + .32)
            diff = _are_aabbs_colliding(aabb, player_aab)
            if diff:
                message_pos.x += diff[0]
                message_pos.y += diff[1]
                message_pos.stance += diff[1]
                message_pos.z += diff[2]
                #assert diff[1] == 0 #TODO: merge with bottom/under checking
                if diff[1] > 0:
                    message_pos.on_ground = True
                    print('Huh?') #TODO
                elif diff[1] < 0:
                    hit_ceiling = True
        return hit_ceiling


    def delete_sector(self, cx, cz):
        del self.csectors[(cx, cz)]


    def get_block_coords(self, x, y, z):
        return (x // 16, z // 16, int(floor(x)) % 16, int(floor(y)), int(floor(z)) % 16)


    def get_block(self, x, y, z):
        cx, cz, ox, oy, oz = self.get_block_coords(x, y, z)
        return self.csectors[cx, cz].get_block(ox, oy, oz)


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
            sector = self.csectors[cx, cz]
        except KeyError:
            self.allocate_sector(cx, cz)
            sector = self.csectors[cx, cz]
        sector.set_block(ox, oy, oz, type, metadata)


    def get_gl_faces(self, pos, fov, ratio, znear, zfar, yaw, pitch):
        renderer = self.renderer

        oldtime = time()
        self.renderer.render(pos, fov, ratio, znear, zfar, yaw, pitch)
        newtime = time()
#        print('Time ellapsed: %f' % (newtime - oldtime))
        return renderer.nb_vertices

