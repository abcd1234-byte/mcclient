#ifndef __SECTOR_H__
#define __SECTOR_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "octree.h"

#define     FACE_SOUTH       0x01
#define     FACE_TOP         0x02
#define     FACE_WEST        0x04
#define     FACE_NORTH       0x08
#define     FACE_BOTTOM      0x10
#define     FACE_EAST        0x20

typedef unsigned short Octree[1 + 8 + 64 + 512];

struct Sector
{
    int cx, cz;
    unsigned char blocktypes[16][16][128];
    unsigned char blockdata[16][16][128];
    unsigned char lighting[16][16][128];
    unsigned char blockfaces[16][16][128]; /* bitmask, 2 bits (by block) unused here */
    struct Sector *east, *west, *north, *south;
    Octree octrees[16];
};

struct Sector *sector_new(int cx, int cz);
void sector_set_block(struct Sector *sector,
                      short x, short y, short z,
                      char blocktype, char blockdata);
void sector_gen_faces(struct Sector *sector);
unsigned int sector_count_faces(struct Sector *sector);
void sector_set_chunk(struct Sector *sector,
                      short ox, short oy, short oz,
                      short size_x, short size_y, short size_z,
                      const unsigned char *data);
void get_sector_coords(int *x, int *y, int *z, int *cx, int *cz);
void sector_update_boundaries(struct Sector *sector);
bool get_block(struct Sector *sector, short x, short z,
               struct Sector **block_sector,
               short *block_x, short *block_z);

#endif
