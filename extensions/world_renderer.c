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

#include "world_renderer.h"
#include "sector.h"
#include "blocktypes.h"
#include "math.h"
#include "frustum.h"


#define         BOTTOM      0 // -y
#define         TOP         1 // +y
#define         EAST        2 // -z
#define         WEST        3 // +z
#define         NORTH       4 // -x
#define         SOUTH       5 // +x


struct WorldRenderer *world_renderer_new(void)
{
    struct WorldRenderer *world_renderer = calloc(1, sizeof(struct WorldRenderer));
    return world_renderer;
}


void world_renderer_reset_rendering(struct WorldRenderer *world_renderer)
{
    world_renderer->nb_vertices = 0;
    world_renderer->nb_alpha_vertices = 0;
}

inline static bool _is_cube_visible(struct ViewContext *view_context,
                                    unsigned short size,
                                    unsigned char *left_to_test,
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


/*
      F──────G
     ╱┊     ╱│
    B——————C │
    │ E┈┈┈┈│┈H
    │/     │╱
    A──────D
*/

#define     CORNER_A        {0., 0., 0.}
#define     CORNER_B        {0., 1., 0.}
#define     CORNER_C        {1., 1., 0.}
#define     CORNER_D        {1., 0., 0.}

#define     CORNER_E        {0., 0., 1.}
#define     CORNER_F        {0., 1., 1.}
#define     CORNER_G        {1., 1., 1.}
#define     CORNER_H        {1., 0., 1.}

inline static void _render_face(struct vertexattrib *vertices,
                                unsigned int *nb_vertices,
                                struct WorldRenderer *world_renderer,
                                struct Sector *sector,
                                unsigned short x, unsigned short y, unsigned short z,
                                int abs_x, int abs_y, int abs_z,
                                char nx, char ny, char nz,
                                unsigned char face)
{
    short x2, y2, z2;
    unsigned char orientation = 0;
    unsigned char uv[2] = {0, 0};

    static const float lightlevels[] = {0.0351843720888, .04398046511104, .0549755813888,
                                        .068719476736, .08589934592, .1073741824,
                                        .134217728, .16777216, .2097152, .262144,
                                        .32768, .4096, .512, .64, .8, 1};


    static const struct vertexattrib faces[6][4] = {
        [BOTTOM] = {CORNER_A, CORNER_E, CORNER_H, CORNER_D},
        [TOP] = {CORNER_B, CORNER_C, CORNER_G, CORNER_F},
        [EAST] = {CORNER_A, CORNER_D, CORNER_C, CORNER_B},
        [WEST] = {CORNER_H, CORNER_E, CORNER_F, CORNER_G},
        [NORTH] = {CORNER_E, CORNER_A, CORNER_B, CORNER_F},
        [SOUTH] = {CORNER_D, CORNER_H, CORNER_G, CORNER_C}};

    struct Sector *light_sector = NULL;

    // Safety net:
    if ((*nb_vertices) == MAX_VERTICES)
        return;

    vertices[3] = faces[face][0];
    vertices[2] = faces[face][1];
    vertices[1] = faces[face][2];
    vertices[0] = faces[face][3];

    // Color (lighting) calculation:
    y2 = y + ny;
    if (0 <= y2 && y <= 127 && get_block(sector, x + nx, z + nz, &light_sector, &x2, &z2))
    {
        unsigned char llevel1 = light_sector->lighting[x2][z2][y2] & 0x0F;
        unsigned char llevel2 = (light_sector->lighting[x2][z2][y2] >> 4);
        float color_value = lightlevels[(llevel1 > llevel2) ? llevel1 : llevel2];
        vertices[0].r = vertices[0].g = vertices[0].b = color_value;
        vertices[1].r = vertices[1].g = vertices[1].b = color_value;
        vertices[2].r = vertices[2].g = vertices[2].b = color_value;
        vertices[3].r = vertices[3].g = vertices[3].b = color_value;
    }

    // Annoying texture calculation
    if (blocktypes[sector->blocktypes[x][z][y]].texfunc == NULL)
    {
        uv[0] = blocktypes[sector->blocktypes[x][z][y]].texcoords.u;
        uv[1] = blocktypes[sector->blocktypes[x][z][y]].texcoords.v;
        orientation = 0;

        vertices[0].u = uv[0] / 16. + uvcorners[orientation][0];
        vertices[0].v = (15 - uv[1]) / 16. + uvcorners[orientation][1];

        orientation += 1;

        vertices[1].u = uv[0] / 16. + uvcorners[orientation][0];
        vertices[1].v = (15 - uv[1]) / 16. + uvcorners[orientation][1];

        orientation += 1;

        vertices[2].u = uv[0] / 16. + uvcorners[orientation][0];
        vertices[2].v = (15 - uv[1]) / 16. + uvcorners[orientation][1];

        orientation += 1;

        vertices[3].u = uv[0] / 16. + uvcorners[orientation][0];
        vertices[3].v = (15 - uv[1]) / 16. + uvcorners[orientation][1];
    }
    else
    {
        if (!blocktypes[sector->blocktypes[x][z][y]].texfunc(x, y, z, sector,
                                                             face, vertices))
            return;
    }

    *nb_vertices += 4;

    vertices[0].x += abs_x; vertices[0].y += abs_y; vertices[0].z += abs_z;
    vertices[1].x += abs_x; vertices[1].y += abs_y; vertices[1].z += abs_z;
    vertices[2].x += abs_x; vertices[2].y += abs_y; vertices[2].z += abs_z;
    vertices[3].x += abs_x; vertices[3].y += abs_y; vertices[3].z += abs_z;
}


inline static void world_renderer_render_face(struct WorldRenderer *world_renderer,
                                 struct Sector *sector,
                                 struct ViewContext *view_context,
                                 unsigned short x, unsigned short y, unsigned short z,
                                 int abs_x, int abs_y, int abs_z,
                                 char nx, char ny, char nz,
                                 unsigned char face)
{
    struct vertexattrib *vertices;

    if (blocktypes[sector->blocktypes[x][z][y]].flags & BLOCKTYPE_FLAG_TRANSPARENT)
    {
        vertices = world_renderer->vertices + (MAX_VERTICES + world_renderer->nb_alpha_vertices);
        _render_face(vertices, &world_renderer->nb_alpha_vertices,
                     world_renderer, sector, x, y, z, abs_x, abs_y, abs_z,
                     nx, ny, nz, face);
    }
    else
    {
        vertices = world_renderer->vertices + world_renderer->nb_vertices;
        _render_face(vertices, &world_renderer->nb_vertices,
                     world_renderer, sector, x, y, z, abs_x, abs_y, abs_z,
                     nx, ny, nz, face);
    }
}

inline static void world_renderer_render_block(struct WorldRenderer *world_renderer,
                                 struct Sector *sector,
                                 struct ViewContext *view_context,
                                 unsigned short x, unsigned short y, unsigned short z)
{
    int abs_x, abs_z;
    double dir_x, dir_y, dir_z;
    unsigned char faces = sector->blockfaces[x][z][y];
    struct BlockType type = blocktypes[sector->blocktypes[x][z][y]];

    if (type.flags & BLOCKTYPE_FLAG_NONBLOCK && type.drawfunc != NULL)
    {
        type.drawfunc(world_renderer, sector, view_context, x, y, z);
        return;
    }

    struct Vec3D southwesttop = CORNER_G;
    struct Vec3D northeastbottom = CORNER_A;

    abs_x = (sector->cx << 4) | x;
    abs_z = (sector->cz << 4) | z;

    dir_x = abs_x - view_context->x;
    dir_y = y - view_context->y;
    dir_z = abs_z - view_context->z;

    southwesttop.x += dir_x;
    southwesttop.y += dir_y;
    southwesttop.z += dir_z;

    northeastbottom.x += dir_x;
    northeastbottom.y += dir_y;
    northeastbottom.z += dir_z;

    if (faces & FACE_SOUTH && southwesttop.x < 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   1, 0, 0,
                                   SOUTH);
    if (faces & FACE_NORTH && northeastbottom.x > 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   -1, 0, 0,
                                   NORTH);
    if (faces & FACE_WEST && southwesttop.z < 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   0, 0, 1,
                                   WEST);
    if (faces & FACE_EAST && northeastbottom.z > 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   0, 0, -1,
                                   EAST);
    if (faces & FACE_TOP && southwesttop.y < 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   0, 1, 0,
                                   TOP);
    if (faces & FACE_BOTTOM && northeastbottom.y > 0)
        world_renderer_render_face(world_renderer, sector, view_context,
                                   x, y, z, abs_x, y, abs_z,
                                   0, -1, 0,
                                   BOTTOM);
}


void world_renderer_render_octree_notest(struct WorldRenderer *world_renderer,
                                         struct Sector *sector, Octree octree,
                                         struct ViewContext *view_context,
                                         unsigned short idx, unsigned short size,
                                         unsigned short x, unsigned short y, unsigned short z)
{
    static const unsigned char dirs[8][3] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
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
    static const unsigned char dirs[8][3] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
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
                                                      (sector->cx << 4) | x2, y2,
                                                      (sector->cz << 4) | z2))
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


struct SortItem {
    float distance2;
    unsigned int index;
};


int sort_item_comparison(const void *a, const void *b)
{
    const struct SortItem *item1, *item2;
    item1 = (struct SortItem *) a;
    item2 = (struct SortItem *) b;

    return item2->distance2 - item1->distance2;
}


void sort_alpha_faces(struct WorldRenderer *world_renderer,
                      struct ViewContext *view_context)
{
    struct SortItem *items;

    // 1. Build distances list
    items = malloc(sizeof(struct SortItem) * world_renderer->nb_alpha_vertices);
    for (unsigned int i=0; i < world_renderer->nb_alpha_vertices / 4; i++)
    {
        float distance2_min = -1;
        for (unsigned int j=0; j < 4; j++)
        {
            struct vertexattrib vertex;
            float dx, dy, dz;
            float distance2;
            vertex = world_renderer->vertices[MAX_VERTICES + (i * 4 + j)];
            dx = vertex.x - view_context->x;
            dy = vertex.y - view_context->y;
            dz = vertex.z - view_context->z;
            distance2 = dx * dx + dy * dy + dz * dz;
            if (distance2_min < 0 || distance2 < distance2_min)
                distance2_min = distance2;
        }
        items[i].index = i;
        items[i].distance2 = distance2_min;
    }

    // 2. Sort indices
    qsort(items, world_renderer->nb_alpha_vertices / 4, sizeof(struct SortItem),
          sort_item_comparison);

    // 3. Reconstruct array
    struct vertexattrib *vertices = malloc(sizeof(struct vertexattrib) * world_renderer->nb_alpha_vertices);

    memcpy(vertices, world_renderer->vertices + MAX_VERTICES, sizeof(struct vertexattrib) * world_renderer->nb_alpha_vertices);

    for (unsigned int i=0; i < world_renderer->nb_alpha_vertices / 4; i++)
    {
        memcpy(world_renderer->vertices + MAX_VERTICES + i * 4,
               vertices + items[i].index * 4, 4 * sizeof(struct vertexattrib));
    }

    // 4. Cleanup
    free(vertices);
    free(items);
}

