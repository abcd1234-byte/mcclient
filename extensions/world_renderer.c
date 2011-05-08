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
                                    unsigned short size, unsigned char *left_to_test,
                                    int x, int y, int z)
{
    for (unsigned int i=0; i < 6; i++)
    {
        if ((*left_to_test) & (1 << i))
        {
            struct Vec3D p_vertex = {x, y, z};
            struct Vec3D n_vertex = p_vertex;

            if (view_context->frustum[i].a >= 0)
                p_vertex.x += size;
            else
                n_vertex.x += size;
            if (view_context->frustum[i].b >= 0)
                p_vertex.y += size;
            else
                n_vertex.y += size;
            if (view_context->frustum[i].c >= 0)
                p_vertex.z += size;
            else
                n_vertex.z += size;

            // If yes, it's outside for sure!
            if (wrong_side_of_plane(view_context->frustum[i], p_vertex))
                return false;

            // If yes, it (and its children) is on the right side of the plane for sure!
            if (right_side_of_plane(view_context->frustum[i], n_vertex))
                *left_to_test ^= (1 << i);
        }
    }

    return true;
}


void world_renderer_get_texture(struct WorldRenderer *world_renderer,
                                unsigned char blocktype,
                                unsigned char blockdata,
                                unsigned char face,
                                unsigned char *uv,
                                unsigned char *orientation)
{
    //TODO: find another (faster) way to get texture info
    PyObject *arglist = NULL, *result = NULL;
    arglist = Py_BuildValue("(bbb)", blocktype, blockdata, face);
    result = PyObject_CallObject(world_renderer->get_block_texture, arglist);
    if (result != NULL)
    {
        PyArg_ParseTuple(result, "bbb", uv, uv + 1, orientation);
        Py_DECREF(result);
    }
    Py_DECREF(arglist);
}


inline static void world_renderer_render_face(struct WorldRenderer *world_renderer,
                                 struct Sector *sector,
                                 struct ViewContext *view_context,
                                 unsigned short x, unsigned short y, unsigned short z,
                                 int abs_x, int abs_y, int abs_z,
                                 char nx, char ny, char nz,
                                 unsigned char face)
{
    int cx2, cz2, x2, y2, z2;
    unsigned char orientation = 0;
    unsigned char uv[2] = {0, 0};
    float lightlevels[] = {.04398046511104, .0549755813888, .068719476736,
                           .08589934592, .1073741824, .134217728, .16777216,
                           .2097152, .262144, .32768, .4096, .512, .64, .8, 1};
    float corners[8][2] = {{0, 0}, {0, 1./16.}, {1./16., 1./16.}, {1./16., 0},
                           {0, 0}, {0, 1./16.}, {1./16., 1./16.}, {1./16., 0}};
    struct Sector *light_sector = NULL;

    // Safety net:
    if (world_renderer->nb_vertices == MAX_VERTICES)
        return;

    struct vertex *vertices = world_renderer->vertices + world_renderer->nb_vertices;
    struct color *colors = world_renderer->colors + world_renderer->nb_vertices;
    struct uv *texcoords = world_renderer->texcoords + world_renderer->nb_vertices;

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

    // Color (lighting) calculation:
    x2 = abs_x + nx;
    y2 = abs_y + ny;
    z2 = abs_z + nz;

    // Bottleneck #3
    // (Bottleneck #2 is OpenGL/PyOpenGL, VBO and interlaced buffers might help)
    get_sector_coords(&x2, &y2, &z2, &cx2, &cz2);
    light_sector = world_renderer_get_sector(world_renderer, cx2, cz2);

    if (light_sector != NULL && 0 <= y2 && y <= 127)
    {
        float color_value = lightlevels[light_sector->lighting[x2][z2][y2]];
        colors[0].r = colors[0].g = colors[0].b = color_value;
        colors[1].r = colors[1].g = colors[1].b = color_value;
        colors[2].r = colors[2].g = colors[2].b = color_value;
        colors[3].r = colors[3].g = colors[3].b = color_value;
    }

    //TODO: annoying texture calculation
    // Bottleneck #1
    // Not optimal, but... Give calculation to python!
    world_renderer_get_texture(world_renderer, sector->blocktypes[x][z][y],
                                               sector->blockdata[x][z][y], face,
                                               uv, &orientation);

    orientation = (5 - orientation) % 4;

    texcoords[0].u = uv[0] / 16. + corners[orientation][0];
    texcoords[0].v = (15 - uv[1]) / 16. + corners[orientation][1];

    orientation += 1;

    texcoords[1].u = uv[0] / 16. + corners[orientation][0];
    texcoords[1].v = (15 - uv[1]) / 16. + corners[orientation][1];

    orientation += 1;

    texcoords[2].u = uv[0] / 16. + corners[orientation][0];
    texcoords[2].v = (15 - uv[1]) / 16. + corners[orientation][1];

    orientation += 1;

    texcoords[3].u = uv[0] / 16. + corners[orientation][0];
    texcoords[3].v = (15 - uv[1]) / 16. + corners[orientation][1];

    world_renderer->nb_vertices += 4;
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
    int abs_x, abs_z;
    double dir_x, dir_y, dir_z;
    unsigned char faces = sector->blockfaces[x][z][y];
    abs_x = sector->cx * 16 + x;
    abs_z = sector->cz * 16 + z;

    dir_x = abs_x - view_context->x;
    dir_y = y - view_context->y;
    dir_z = abs_z - view_context->z;

    if (faces & FACE_SOUTH && dir_x < 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   1, 0, 0,
                                   5);
    if (faces & FACE_NORTH && dir_x > 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   -1, 0, 0,
                                   4);
    if (faces & FACE_WEST && dir_z < 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   0, 0, 1,
                                   3);
    if (faces & FACE_EAST && dir_z > 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   0, 0, -1,
                                   2);
    if (faces & FACE_TOP && dir_y < 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   0, 1, 0,
                                   1);
    if (faces & FACE_BOTTOM && dir_y > 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   0, -1, 0,
                                   0);
}



void world_renderer_render_octree_notest(struct WorldRenderer *world_renderer,
                                         struct Sector *sector, Octree octree,
                                         struct ViewContext *view_context,
                                         unsigned short idx, unsigned short size,
                                         unsigned short x, unsigned short y, unsigned short z)
{
    unsigned char dirs[8][3] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
                                {1, 1, 0}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}};
    int x2, y2, z2;
    size >>= 1;
    if (size == 1)
    {
        for (unsigned char i=0; i < 8; i++)
        {
            x2 = x + dirs[i][0] * size;
            y2 = y + dirs[i][1] * size;
            z2 = z + dirs[i][2] * size;
            world_renderer_render_block(world_renderer, sector,
                                        view_context, x2, y2, z2);
        }
    }
    else
    {
        unsigned short child_idx;
        for (unsigned char i=0; i < 8; i++)
        {
            x2 = x + dirs[i][0] * size;
            y2 = y + dirs[i][1] * size;
            z2 = z + dirs[i][2] * size;
            child_idx = OCTREE_GET_CHILD(idx, dirs[i][0], dirs[i][1], dirs[i][2]);

            if (octree[child_idx])
                world_renderer_render_octree_notest(world_renderer, sector,
                                                    octree, view_context,
                                                    child_idx, size,
                                                    x2, y2, z2);
        }
    }
}



void world_renderer_render_octree(struct WorldRenderer *world_renderer,
                                  struct Sector *sector, Octree octree,
                                  struct ViewContext *view_context,
                                  unsigned short idx, unsigned short size,
                                  unsigned char left_to_test,
                                  unsigned short x, unsigned short y, unsigned short z)
{
    unsigned char dirs[8][3] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
                                {1, 1, 0}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}};
    int x2, y2, z2;
    size >>= 1;
    if (size == 1)
    {
        for (unsigned char i=0; i < 8; i++)
        {
            unsigned char left_to_test2 = left_to_test;
            x2 = x + dirs[i][0] * size;
            y2 = y + dirs[i][1] * size;
            z2 = z + dirs[i][2] * size;
            if (_is_cube_visible(view_context, size, &left_to_test2, sector->cx * 16 + x2, y2, sector->cz * 16 + (int) z2))
                world_renderer_render_block(world_renderer, sector,
                                            view_context, x2, y2, z2);
        }
    }
    else
    {
        unsigned short child_idx;
        for (unsigned char i=0; i < 8; i++)
        {
            unsigned char left_to_test2 = left_to_test;
            x2 = x + dirs[i][0] * size;
            y2 = y + dirs[i][1] * size;
            z2 = z + dirs[i][2] * size;
            child_idx = OCTREE_GET_CHILD(idx, dirs[i][0], dirs[i][1], dirs[i][2]);

            if (octree[child_idx] && _is_cube_visible(view_context, size,
                                                      &left_to_test2,
                                                      sector->cx * 16 + x2, y2, sector->cz * 16 + z2))
            {
                if (left_to_test2 == 0) // Fully inside!
                    world_renderer_render_octree_notest(world_renderer, sector,
                                                        octree, view_context,
                                                        child_idx, size,
                                                        x2, y2, z2);
                else
                    world_renderer_render_octree(world_renderer, sector, octree,
                                                 view_context, child_idx, size,
                                                 left_to_test2, x2, y2, z2);
            }
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
        unsigned char left_to_test = 63;
        if (_is_cube_visible(view_context, 16, &left_to_test,
                             sector->cx * 16, cy * 16, sector->cz * 16))
            world_renderer_render_octree(world_renderer, sector,
                                         sector->octrees[cy], view_context,
                                         0, 16, left_to_test, 0, cy * 16, 0);
    }
}
