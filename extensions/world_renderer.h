#ifndef __WORLD_RENDERER_H__
#define __WORLD_RENDERER_H__

#include <Python.h>

#include "sector.h"
#include "frustum.h"

#define MAX_VERTICES        (4 * 32768)


struct vertex {
    float x;
    float y;
    float z;
};

struct color {
    float r;
    float g;
    float b;
};

struct uv {
    float u;
    float v;
};

struct ViewContext {
    double x, y, z;
    struct Plane frustum[6];
};

struct WorldRenderer {
    PyObject *sectors_dict, *get_block_texture;
    unsigned int nb_vertices;
    unsigned int nb_alpha_vertices;
    struct vertex vertices[MAX_VERTICES * 2];
    struct color colors[MAX_VERTICES * 2];
    struct uv texcoords[MAX_VERTICES * 2];
};


struct WorldRenderer *world_renderer_new(void);
void world_renderer_reset_rendering(struct WorldRenderer *world_renderer);
void world_renderer_render_sector(struct WorldRenderer *world_renderer, struct Sector *sector, struct ViewContext *view_context);
struct Sector *world_renderer_get_sector(struct WorldRenderer *world_renderer, int cx, int cz);
void sort_alpha_faces(struct WorldRenderer *world_renderer, struct ViewContext *view_context);

#endif
