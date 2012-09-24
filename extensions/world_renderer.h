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

#ifndef __WORLD_RENDERER_H__
#define __WORLD_RENDERER_H__

#include "sector.h"
#include "frustum.h"

#define MAX_VERTICES        (8 * 65536)


struct vertexattrib {
    float x, y, z;
    float u, v;
    float r, g, b;
};

struct ViewContext {
    double x, y, z;
    struct Plane frustum[6];
};

struct WorldRenderer {
    unsigned int nb_vertices;
    unsigned int nb_alpha_vertices;
    struct vertexattrib vertices[MAX_VERTICES * 2];
};


struct WorldRenderer *world_renderer_new(void);
void world_renderer_reset_rendering(struct WorldRenderer *world_renderer);
void world_renderer_render_sector(struct WorldRenderer *world_renderer, struct Sector *sector, struct ViewContext *view_context);
void sort_alpha_faces(struct WorldRenderer *world_renderer, struct ViewContext *view_context);

#endif
