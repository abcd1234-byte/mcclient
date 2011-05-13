/*
 * Copyright (C) 2011 Thibaut GIRKA <thib@sitedethib.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 3 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __BLOCKTYPES_H__
#define __BLOCKTYPES_H__

#include <stdlib.h>
#include "world_renderer.h"

#define     BLOCKTYPE_FLAG_NONBLOCK     1
#define     BLOCKTYPE_FLAG_USABLE       2
#define     BLOCKTYPE_FLAG_TRANSPARENT  4
#define     BLOCKTYPE_FLAG_NONSOLID     8
#define     BLOCKTYPE_FLAG_ALLFACES     16

typedef bool (*TextureFunc) (unsigned short x, unsigned short y, unsigned short z,
                             struct Sector *sector, unsigned char face,
                             struct vertex *vertices,
                             struct uv *texcoords, struct color *colors);

struct BlockType {
    const char *name;
    unsigned short flags;
    struct {
        unsigned char u;
        unsigned char v;
    } texcoords;
    TextureFunc texfunc; //TODO: union for non-block blocks
};

extern struct BlockType blocktypes[256];
extern float uvcorners[8][2];

#endif
