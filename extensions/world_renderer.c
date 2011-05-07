#include "world_renderer.h"
#include "sector.h"
#include "sector_py.h"
#include "math.h"


struct WorldRenderer *world_renderer_new(void)
{
    struct WorldRenderer *world_renderer = calloc(1, sizeof(struct WorldRenderer));
    return world_renderer;
}


void world_renderer_reset_rendering(struct WorldRenderer *world_renderer)
{
    world_renderer->nb_vertices = 0;
}

inline static bool _is_cube_visible(struct ViewContext *view_context,
                                    unsigned short size,
                                    int x, int y, int z)
{
    //TODO
    float center_x, center_y, center_z;
    float dist;
    center_x = x + (float) size / 2;
    center_y = y + (float) size / 2;
    center_z = z + (float) size / 2;

    return ((abs(center_y - view_context->y) - size / 2 < view_context->dist)
         && (abs(center_x - view_context->x) - size / 2 < view_context->dist)
         && (abs(center_z - view_context->z) - size / 2 < view_context->dist));
}


inline static void world_renderer_render_face(struct WorldRenderer *world_renderer,
                                 struct Sector *sector,
                                 struct ViewContext *view_context,
                                 unsigned short x, unsigned short y, unsigned short z,
                                 int abs_x, int abs_y, int abs_z,
                                 char nx, char ny, char nz)
{
    int cx2, cz2, x2, y2, z2;
    struct Sector *light_sector = NULL;

    // Safety net:
    if (world_renderer->nb_vertices == MAX_VERTICES)
        return;

    struct vertex *vertices = world_renderer->vertices + world_renderer->nb_vertices;
    struct color *colors = world_renderer->colors + world_renderer->nb_vertices;

    //TODO: backface culling
    if ((abs_x - view_context->x) * nx + (abs_y - view_context->y) * ny + (abs_z - view_context->z) * nz >= 0)
        return;

    //TODO: vertex calculation
    if (nx != 0)
    {
        vertices[0].y = abs_y;      vertices[0].z = abs_z;
        vertices[1].y = abs_y + 1;  vertices[1].z = abs_z;
        vertices[2].y = abs_y + 1;  vertices[2].z = abs_z + 1;
        vertices[3].y = abs_y;      vertices[3].z = abs_z + 1;

        if (nx == 1)
            vertices[0].x = vertices[1].x = vertices[2].x = vertices[3].x = abs_x + 1;
        else
            vertices[0].x = vertices[1].x = vertices[2].x = vertices[3].x = abs_x;
    }
    else if (ny != 0)
    {
        vertices[0].x = abs_x;      vertices[0].z = abs_z;
        vertices[1].x = abs_x;      vertices[1].z = abs_z + 1;
        vertices[2].x = abs_x + 1;  vertices[2].z = abs_z + 1;
        vertices[3].x = abs_x + 1;  vertices[3].z = abs_z;

        if (ny == 1)
            vertices[0].y = vertices[1].y = vertices[2].y = vertices[3].y = abs_y + 1;
        else
            vertices[0].y = vertices[1].y = vertices[2].y = vertices[3].y = abs_y;
    }
    else if (nz != 0)
    {
        vertices[0].x = abs_x;      vertices[0].y = abs_y;
        vertices[1].x = abs_x;      vertices[1].y = abs_y + 1;
        vertices[2].x = abs_x + 1;  vertices[2].y = abs_y + 1;
        vertices[3].x = abs_x + 1;  vertices[3].y = abs_y;

        if (nz == 1)
            vertices[0].z = vertices[1].z = vertices[2].z = vertices[3].z = abs_z + 1;
        else
            vertices[0].z = vertices[1].z = vertices[2].z = vertices[3].z = abs_z;
    }
    else
        printf("WTF!?!\n");

    //TODO: color calculation: look AROUND, NOT HERE
//    PyObject *arglist = Py_BuildValue("(ii"), cx, cz);
//    result = PyObject_CallObject(..., arglist);
//    world_renderer->world
//    Py_DECREF(arglist);

    x2 = abs_x + nx;
    y2 = abs_y + ny;
    z2 = abs_z + nz;

    get_sector_coords(&x2, &y2, &z2, &cx2, &cz2);
    light_sector = world_renderer_get_sector(world_renderer, cx2, cz2);

    if (light_sector != NULL && 0 <= y2 && y <= 127)
    {
        float color_value = powf(0.8f, (15 - light_sector->lighting[x2][z2][y2]));
        colors[0].r = colors[0].g = colors[0].b = color_value; //TODO
        colors[1].r = colors[1].g = colors[1].b = color_value; //TODO
        colors[2].r = colors[2].g = colors[2].b = color_value; //TODO
        colors[3].r = colors[3].g = colors[3].b = color_value; //TODO
    }

    world_renderer->nb_vertices += 4;
    //TODO: annoying texture calculation
}


struct Sector *world_renderer_get_sector(struct WorldRenderer *world_renderer,
                                         int cx, int cz)
{
    struct Sector *sector;
    PyObject *key = NULL, *result = NULL;

    key = Py_BuildValue("(ii)", cx, cz);
    result = PyObject_GetItem(world_renderer->sectors_dict, key);
    Py_DECREF(key);

    if (result == NULL)
        return NULL;

    /* TODO: Check type
    if (!PyObject_TypeCheck(result, SectorType))
    {
        Py_DECREF(result);
        return NULL;
    }*/

    sector = ((Sector *) result)->sector;
    Py_DECREF(result);

    return sector;
}


inline static void world_renderer_render_block(struct WorldRenderer *world_renderer,
                                 struct Sector *sector,
                                 struct ViewContext *view_context,
                                 unsigned short x, unsigned short y, unsigned short z)
{
    int abs_x, abs_y, abs_z;
    unsigned char faces = sector->blockfaces[x][z][y];
    abs_x = sector->cx * 16 + x;
    abs_y = y;
    abs_z = sector->cz * 16 + z;
    if (faces & FACE_SOUTH)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, abs_y, abs_z,
                                   1, 0, 0);
    if (faces & FACE_NORTH)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, abs_y, abs_z,
                                   -1, 0, 0);
    if (faces & FACE_WEST)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, abs_y, abs_z,
                                   0, 0, 1);
    if (faces & FACE_EAST)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, abs_y, abs_z,
                                   0, 0, -1);
    if (faces & FACE_TOP)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, abs_y, abs_z,
                                   0, 1, 0);
    if (faces & FACE_BOTTOM)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, abs_y, abs_z,
                                   0, -1, 0);
}


void world_renderer_render_octree(struct WorldRenderer *world_renderer,
                                  struct Sector *sector, Octree octree,
                                  struct ViewContext *view_context,
                                  unsigned short idx, unsigned short size,
                                  unsigned short x, unsigned short y, unsigned short z)
{
    unsigned char dirs[8][3] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
                                {1, 1, 0}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}};
    size /= 2;
    if (size == 1)
    {
        for (unsigned char i=0; i < 8; i++)
        {
            int x2, y2, z2;
            x2 = x + dirs[i][0] * size;
            y2 = y + dirs[i][1] * size;
            z2 = z + dirs[i][2] * size;
            if (_is_cube_visible(view_context, size, sector->cx * 16 + x2, y2, sector->cz * 16 + (int) z2))
                world_renderer_render_block(world_renderer, sector,
                                            view_context, x2, y2, z2);
        }
    }
    else
    {
        for (unsigned char i=0; i < 8; i++)
        {
            int x2, y2, z2;
            unsigned short child_idx;
            x2 = x + dirs[i][0] * size;
            y2 = y + dirs[i][1] * size;
            z2 = z + dirs[i][2] * size;
            child_idx = OCTREE_GET_CHILD(idx, dirs[i][0], dirs[i][1], dirs[i][2]);

            if (octree[child_idx] && _is_cube_visible(view_context, size, sector->cx * 16 + x2, y2, sector->cz * 16 + z2))
                world_renderer_render_octree(world_renderer, sector, octree,
                                             view_context, child_idx, size, x2, y2, z2);
        }
    }
}


void world_renderer_render_sector(struct WorldRenderer *world_renderer,
                                  struct Sector *sector,
                                  struct ViewContext *view_context)
{
    // Per-octree loop
    for (unsigned short cy=0; cy < 16; cy++)
    {
        //TODO: additional tests
        if (_is_cube_visible(view_context, 16, sector->cx * 16, cy * 16, sector->cz * 16))
            world_renderer_render_octree(world_renderer, sector, sector->octrees[cy], view_context, 0, 16, 0, cy * 16, 0);
    }
}
