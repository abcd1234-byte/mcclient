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
from math import cos, sin, radians

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



class Player(object):
    def __init__(self):
        self.message_pos = None
        self.speed = [0, 0, 0]



class MCClient(object):
    def __init__(self, name, screen_size, server='localhost', fullscreen=False,
                 fovy=60, zfar=100):
        # Set up display
        pygame.init()

        flags = pygame.OPENGL | pygame.DOUBLEBUF | pygame.HWSURFACE
        if fullscreen:
            flags |= pygame.FULLSCREEN

        self.zfar, self.fovy = zfar, fovy
        self.width, self.height = screen_size
        self.aspect_ratio = float(self.width) / float(self.height)
        self.window = pygame.display.set_mode(screen_size, flags)

        # Set up GL
        # 1. Hints and enables/disables: FIXME (programmable pipeline)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_BLEND)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glEnable(GL_TEXTURE_2D)
        glDisable(GL_LIGHTING) #TODO
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
        #TODO: programmable pipeline
        glEnable(GL_FOG)
        glFogi(GL_FOG_MODE, GL_LINEAR)
        # 2. VBO
        self.vbo = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.vbo)
        # 3. ClientStates
        glEnableClientState(GL_VERTEX_ARRAY)
        glEnableClientState(GL_TEXTURE_COORD_ARRAY)
        glEnableClientState(GL_COLOR_ARRAY)
        # 4. Projections: FIXME (programmable pipeline)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(fovy, self.aspect_ratio, 0.1, zfar)

        #TODO: grab mouse?
        #pygame.event.set_grab(True)
        #pygame.mouse.set_visible(False)

        # Load textures
        terrain = loadImage('mcdata/terrain.png')

        # Instantiate world and connection
        self.world = World()
        self.connection = Connection(name)
        self.connection.connect(server)
        messages.KeepAlive().send(self.connection.socket) #TODO: Ok? Needed?

        self.player = Player()


    def handle_messages(self):
        for message in self.connection.get_messages():
            if type(message) == messages.ChatMessage:
                #TODO: display on screen
                print('[MSG] %s' % message.message)
            elif type(message) == messages.BlockChange:
                self.world.modify_block(message.x, message.y, message.z,
                                        message.block_type,
                                        message.block_metadata)
            elif type(message) == messages.MultiBlockChange:
                for (ox, oy, oz), type_, metadata in zip(message.coordinates,
                                                         message.types,
                                                         message.metadatas):
                    ox = ox + message.chunk_x * 16
                    oz = oz + message.chunk_z * 16
                    self.world.modify_block(ox, oy, oz, type_, metadata)
            elif type(message) == messages.PreChunk:
                if message.mode == 1:
                    self.world.allocate_sector(message.x, message.z)
                else:
                    self.world.delete_sector(message.x, message.z)
            elif type(message) == messages.MapChunk:
                self.world.map_chunk(message.x, message.y, message.z,
                                     message.size_x, message.size_y,
                                     message.size_z, message.compressed_data)
            elif type(message) == messages.PlayerPosLook:
                print('[PlayerPosLook] %f, %f (%f), %f ; %f, %f'
                        % (message.x, message.y, message.stance, message.z,
                           message.yaw, message.pitch))
                self.player.message_pos = message
                message.send(self.connection.socket)
            elif type(message) == messages.UpdateHealth:
                print('[Vie] %d / 20' % message.health)
                if message.health <= 0:
                    messages.Respawn(0).send(self.connection.socket)
            elif type(message) == messages.Disconnect:
                print('[Kicked] %s' % message.reason)
                self.running = False


    def handle_events(self):
        # Pygame events
        for event in pygame.event.get():
            if event.type == pygame.MOUSEMOTION:
                if self.player.message_pos:
                    self.player.message_pos.yaw += event.rel[0] / 2.
                    self.player.message_pos.pitch += event.rel[1] / 3.
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    self.running = False
                elif event.key == pygame.K_z:
                    self.player.speed[2] = 4.
                elif event.key == pygame.K_s:
                    self.player.speed[2] = -4.
                elif event.key == pygame.K_SPACE and self.player.message_pos.on_ground:
                    self.player.speed[1] = 8.
            elif event.type == pygame.KEYUP:
                if event.key in (pygame.K_z, pygame.K_s):
                    self.player.speed[2] = 0.


    def run(self):
        clock = pygame.time.Clock()

        self.running = True
        while self.connection.socket and self.running:
            delta = clock.get_time() / 1000.

            self.handle_messages()
            self.handle_events()

            if self.player.message_pos:
                self.player.message_pos.y += self.player.speed[1] * delta
                self.player.message_pos.stance += self.player.speed[1] * delta
                self.player.message_pos.z += self.player.speed[2] * cos(radians(self.player.message_pos.yaw)) * delta
                self.player.message_pos.x -= self.player.speed[2] * sin(radians(self.player.message_pos.yaw)) * delta

            # TODO: check for collision
            if self.player.message_pos:
                self.player.message_pos.on_ground = True
                hit_ceiling = self.world.collide(self.player.message_pos)
                if hit_ceiling:
                    self.player.speed[1] = min(0, self.player.speed[1])
                if self.player.message_pos.on_ground:
                    self.player.speed[1] = 0
                else:
                    self.player.speed[1] = max(self.player.speed[1] - delta * 30, -12.)

            if not self.player.message_pos:
                messages.KeepAlive().send(self.connection.socket) #TODO: Ok? Needed?
            else:
                self.player.message_pos.pitch = max(-90, min(90, self.player.message_pos.pitch))

                self.player.message_pos.send(self.connection.socket)

                pos = self.player.message_pos.x, self.player.message_pos.stance, self.player.message_pos.z

                glClearColor(0.2, 0.2, 0.8, 0)
                glFogfv(GL_FOG_COLOR, (0.2, 0.2, 0.8))
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

                #TODO: new pipeline
                glMatrixMode(GL_MODELVIEW)
                glLoadIdentity()
                glRotated(self.player.message_pos.pitch, 1, 0, 0)
                glRotated(180 + self.player.message_pos.yaw, 0, 1, 0)
                glTranslated(-self.player.message_pos.x, -self.player.message_pos.stance, -self.player.message_pos.z)

                nb_vertices = self.world.get_gl_faces(pos, self.fovy,
                                                      self.aspect_ratio,
                                                      0.1, self.zfar,
                                                      self.player.message_pos.yaw,
                                                      self.player.message_pos.pitch)
                glFogf(GL_FOG_START, self.zfar * 2 / 3)
                glFogf(GL_FOG_END, self.zfar)

                glVertexPointer(3, GL_FLOAT, 32, c_void_p(0))
                glTexCoordPointer(2, GL_FLOAT, 32, c_void_p(12))
                glColorPointer(3, GL_FLOAT, 32, c_void_p(20))

                #Uncomment to check backface culling
                #glPolygonMode(GL_FRONT, GL_POINT)
                glDrawArrays(GL_QUADS, 0, nb_vertices)
                glDrawArrays(GL_QUADS, 0, nb_vertices)

            pygame.display.flip()
            clock.tick(60)

        if not self.running:
            messages.Disconnect().send(self.connection.socket)


if __name__ == '__main__':
    client = MCClient('Bot2', (800, 600))
    client.run()

