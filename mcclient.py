#!/usr/bin/env python

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

import pygame
import OpenGL
OpenGL.FORWARD_COMPATIBLE_ONLY = True
from OpenGL.GL import *
from OpenGL.GLU import *
from ctypes import c_void_p
from math import cos, sin, radians, ceil


from time import time
import network.messages as messages
from network.connection import Connection
from world import World



def loadImage(image):
    textureSurface = pygame.image.load(image)

    textureData = pygame.image.tostring(textureSurface, 'RGBA', 1)

    width = textureSurface.get_width()
    height = textureSurface.get_height()

    texture = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, texture)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, textureData)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)

    return texture, width, height


if __name__ == '__main__':
    pygame.init()
    pygame.display.set_mode((800, 600), pygame.OPENGL | pygame.DOUBLEBUF)

    # VBO
    vbo = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, vbo)

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
    glEnable(GL_DEPTH_TEST)
#    glEnable(GL_ALPHA_TEST)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glEnable(GL_TEXTURE_2D)
    glDisable(GL_LIGHTING)
    glEnableClientState(GL_VERTEX_ARRAY)
    glEnableClientState(GL_TEXTURE_COORD_ARRAY)
    glEnableClientState(GL_COLOR_ARRAY)

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(60, 800./600., 0.1, 128)

#    pygame.event.set_grab(True)
#    pygame.mouse.set_visible(False)

    truc = loadImage('mcdata/terrain.png')

    world = World()
    con  = Connection('Bot2')
    con.connect('localhost')
    messages.KeepAlive().send(con.socket)

    message_pos = None
    speed_y = 0

    clock = pygame.time.Clock()

    quit = False

    old_time = time()
    while con.socket and not quit:
        new_time = time()
        delta = (new_time - old_time)
        old_time = new_time

        msgs = con.get_messages()
        for message in msgs:
            if type(message) == messages.ChatMessage:
                print('[MSG] %s' % message.message)

            if type(message) == messages.BlockChange:
                world.modify_block(message.x, message.y, message.z, message.block_type, message.block_metadata)

            if type(message) == messages.MultiBlockChange:
                for (ox, oy, oz), block_type, block_metadata in zip(message.coordinates, message.types, message.metadatas):
                    ox = ox + message.chunk_x * 16
                    oz = oz + message.chunk_z * 16
                    world.modify_block(ox, oy, oz, block_type, block_metadata)

            if type(message) == messages.PreChunk:
                if message.mode == 1:
                    world.allocate_sector(message.x, message.z)
                else:
                    world.delete_sector(message.x, message.z)

            if type(message) == messages.MapChunk:
                world.map_chunk(message.x, message.y, message.z, message.size_x, message.size_y, message.size_z, message.compressed_data)


            if type(message) == messages.PlayerPosLook:
                print('[PlayerPosLook] %f, %f (%f), %f ; %f, %f'
                        % (message.x, message.y, message.stance, message.z,
                           message.yaw, message.pitch))
                message_pos = message
                message.send(con.socket)

            if type(message) == messages.UpdateHealth:
                print('[Vie] %d / 20' % message.health)
                if message.health <= 0:
                    messages.Respawn(0).send(con.socket)

            if type(message) == messages.Disconnect:
                print('[Kicked] %s' % message.reason)
                quit = True


        # Pygame events
        for event in pygame.event.get():
            if event.type == pygame.MOUSEMOTION:
                if message_pos:
                    message_pos.yaw += event.rel[0] / 2.
                    message_pos.pitch += event.rel[1] / 3.
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    quit = True
        # Keyboard
        key_pressed = pygame.key.get_pressed()
        # TODO: Move accordingly
        if key_pressed[pygame.K_z]:
            #TODO: speed, not pos
            message_pos.z += cos(radians(message_pos.yaw)) * 4 * delta
            message_pos.x -= sin(radians(message_pos.yaw)) * 4 * delta
        if key_pressed[pygame.K_s]:
            #TODO: speed, not pos
            message_pos.z -= cos(radians(message_pos.yaw)) * 3 * delta
            message_pos.x += sin(radians(message_pos.yaw)) * 3 * delta
        # TODO: jump, if on ground
        if key_pressed[pygame.K_SPACE] and message_pos.on_ground:
            speed_y = 8.

        if message_pos:
            message_pos.y += speed_y * delta
            message_pos.stance += speed_y * delta

        # TODO: check for collision
        if message_pos:
            message_pos.on_ground = True
            hit_ceiling = world.collide(message_pos)
            if hit_ceiling:
                speed_y = min(0, speed_y)
            if message_pos.on_ground:
                speed_y = 0
            else:
                speed_y = max(speed_y - delta * 30, -12.)

        if not message_pos:
            messages.KeepAlive().send(con.socket)
        else:
            message_pos.pitch = max(-90, min(90, message_pos.pitch))

            message_pos.send(con.socket)

            pos = message_pos.x, message_pos.stance, message_pos.z
            nb_vertices, vertices = world.get_gl_faces(pos, 60, 800./600., 0.1, 100, message_pos.yaw, message_pos.pitch)
#            print('Nb faces drawn: %d' % (nb_vertices // 4))

            glClearColor(0.0, 0.0, 1.0, 0)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

            #TODO: new pipeline
            glMatrixMode(GL_MODELVIEW)
            glLoadIdentity()
            glRotated(message_pos.pitch, 1, 0, 0)
            glRotated(180 + message_pos.yaw, 0, 1, 0)
            glTranslated(-message_pos.x, -message_pos.stance, -message_pos.z)

            glBufferData(GL_ARRAY_BUFFER, 32 * nb_vertices, vertices, GL_STATIC_DRAW)

            glVertexPointer(3, GL_FLOAT, 32, c_void_p(0))
            glTexCoordPointer(2, GL_FLOAT, 32, c_void_p(12))
            glColorPointer(3, GL_FLOAT, 32, c_void_p(20))

#Uncomment to check backface culling
#            glPolygonMode(GL_FRONT, GL_POINT)
            glDrawArrays(GL_QUADS, 0, nb_vertices)

        pygame.display.flip()
        clock.tick(50)

    if quit:
        messages.Disconnect().send(con.socket)

