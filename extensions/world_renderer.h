#include <Python.h>

#include "sector.h"

#define MAX_VERTICES        (4 * 40000)


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
    unsigned short dist;
};

struct WorldRenderer {
    PyObject *sectors_dict;
    unsigned int nb_vertices;
    struct vertex vertices[MAX_VERTICES];
    struct color colors[MAX_VERTICES];
    struct uv texcoords[MAX_VERTICES];
};


struct WorldRenderer *world_renderer_new(void);
void world_renderer_reset_rendering(struct WorldRenderer *world_renderer);
void world_renderer_render_sector(struct WorldRenderer *world_renderer, struct Sector *sector, struct ViewContext *view_context);
struct Sector *world_renderer_get_sector(struct WorldRenderer *world_renderer, int cx, int cz);

