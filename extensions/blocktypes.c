#include "blocktypes.h"

#define         BOTTOM      0 // -y
#define         TOP         1 // +y
#define         EAST        2 // -z
#define         WEST        3 // +z
#define         NORTH       4 // -x
#define         SOUTH       5 // +x

float uvcorners[8][2] = {{0, 1./16.}, {1./16., 1./16}, {1./16., 0}, {0, 0},
                         {0, 1./16.}, {1./16., 1./16}, {1./16., 0}, {0, 0}};

bool grass_texfunc(unsigned short x, unsigned short y, unsigned short z,
                   struct Sector *sector, unsigned char face,
                   struct uv *texcoords, struct color *colors)
{
    //TODO
    float u, v;
    unsigned char type = sector->blocktypes[x][z][y];
    if (face == BOTTOM)
    {
        // Dirt
        u = blocktypes[3].texcoords.u;
        v = blocktypes[3].texcoords.v;

        texcoords[0].u = u / 16. + uvcorners[0][0];
        texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

        texcoords[1].u = u / 16. + uvcorners[1][0];
        texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

        texcoords[2].u = u / 16. + uvcorners[2][0];
        texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

        texcoords[3].u = u / 16. + uvcorners[3][0];
        texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];
    }
    else if (face == TOP)
    {
        //TODO: (0, 0) + color
        texcoords[0].u = uvcorners[0][0];
        texcoords[0].v = 15 / 16. + uvcorners[0][1];

        texcoords[1].u = uvcorners[1][0];
        texcoords[1].v = 15 / 16. + uvcorners[1][1];

        texcoords[2].u = uvcorners[2][0];
        texcoords[2].v = 15 / 16. + uvcorners[2][1];

        texcoords[3].u = uvcorners[3][0];
        texcoords[3].v = 15 / 16. + uvcorners[3][1];

        if (type == 2)
        {
            colors[0].r = colors[0].b = 0;
            colors[1].r = colors[1].b = 0;
            colors[2].r = colors[2].b = 0;
            colors[3].r = colors[3].b = 0;
        }
        else
        {
            colors[0].r *= 2;
            colors[0].g *= 2;
            colors[0].b *= 2;
            colors[3] = colors[2] =  colors[1] = colors[0];

        }
    }
    else
    {
        //TODO: blocktypes[type].texcoords
        u = blocktypes[type].texcoords.u;
        v = blocktypes[type].texcoords.v;

        texcoords[0].u = u / 16. + uvcorners[0][0];
        texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

        texcoords[1].u = u / 16. + uvcorners[1][0];
        texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

        texcoords[2].u = u / 16. + uvcorners[2][0];
        texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

        texcoords[3].u = u / 16. + uvcorners[3][0];
        texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];
    }

    return true;
}

bool wood_texfunc(unsigned short x, unsigned short y, unsigned short z,
                  struct Sector *sector, unsigned char face,
                  struct uv *texcoords, struct color *colors)
{
    //TODO
    float u, v;
    unsigned char type = sector->blocktypes[x][z][y];
    if (face == BOTTOM || face == TOP)
    {
        u = blocktypes[type].texcoords.u + 1;
        v = blocktypes[type].texcoords.v;

        texcoords[0].u = u / 16. + uvcorners[0][0];
        texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

        texcoords[1].u = u / 16. + uvcorners[1][0];
        texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

        texcoords[2].u = u / 16. + uvcorners[2][0];
        texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

        texcoords[3].u = u / 16. + uvcorners[3][0];
        texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];
    }
    else
    {
        //TODO: check wood type
        u = blocktypes[type].texcoords.u;
        v = blocktypes[type].texcoords.v;

        texcoords[0].u = u / 16. + uvcorners[0][0];
        texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

        texcoords[1].u = u / 16. + uvcorners[1][0];
        texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

        texcoords[2].u = u / 16. + uvcorners[2][0];
        texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

        texcoords[3].u = u / 16. + uvcorners[3][0];
        texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];
    }

    return true;
}


bool leaves_texfunc(unsigned short x, unsigned short y, unsigned short z,
                    struct Sector *sector, unsigned char face,
                    struct uv *texcoords, struct color *colors)
{
    //TODO
    float u, v;
    unsigned char type = sector->blocktypes[x][z][y];

    //TODO: switch with type
    u = blocktypes[type].texcoords.u;
    v = blocktypes[type].texcoords.v;

    texcoords[0].u = u / 16. + uvcorners[0][0];
    texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

    texcoords[1].u = u / 16. + uvcorners[1][0];
    texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

    texcoords[2].u = u / 16. + uvcorners[2][0];
    texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

    texcoords[3].u = u / 16. + uvcorners[3][0];
    texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];

    //TODO: switch with type and decay
    colors[0].r = colors[0].b = 0;
    colors[1].r = colors[1].b = 0;
    colors[2].r = colors[2].b = 0;
    colors[3].r = colors[3].b = 0;

    return true;
}


bool dispenser_texfunc(unsigned short x, unsigned short y, unsigned short z,
                       struct Sector *sector, unsigned char face,
                       struct uv *texcoords, struct color *colors)
{
    float u, v;
    unsigned char type = sector->blocktypes[x][z][y];

    if (face == TOP || face == BOTTOM)
    {
        u = 14;
        v = 3;
    }
    else if (face == sector->blockdata[x][z][y])
    {
        u = blocktypes[type].texcoords.u;
        v = blocktypes[type].texcoords.v;
    }
    else
    {
        u = 13;
        v = 2;
    }

    texcoords[0].u = u / 16. + uvcorners[0][0];
    texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

    texcoords[1].u = u / 16. + uvcorners[1][0];
    texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

    texcoords[2].u = u / 16. + uvcorners[2][0];
    texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

    texcoords[3].u = u / 16. + uvcorners[3][0];
    texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool bed_texfunc(unsigned short x, unsigned short y, unsigned short z,
                 struct Sector *sector, unsigned char face,
                 struct uv *texcoords, struct color *colors)
{
    float u, v;
    unsigned char type = sector->blocktypes[x][z][y];
    unsigned char orientation_to_face[] = {WEST, NORTH, EAST, SOUTH};
    unsigned char orientation;

    orientation = sector->blockdata[x][z][y] & 7;

    if (face == TOP || face == BOTTOM)
    {
        //TODO: check check check
        if (sector->blockdata[x][z][y] & 8) // Head
        {
            u = 7;
            v = 8;
            orientation += 1;
        }
        else
        {
            u = 6;
            v = 8;
            orientation = (orientation + 1) % 4;
        }
    }
    else
    {
        if (sector->blockdata[x][z][y] & 8) // Head
        {
            if (orientation_to_face[orientation] == face)
                u = 8;
            else
                u = 7;
            v = 9;
            //TODO: orientation
        }
        else
        {
            if (orientation_to_face[(orientation + 2) % 4] == face)
                u = 5;
            else
                u = 6;
            v = 9;
            //TODO: orientation
        }
        orientation = 0;
        //TODO: unused faces (between the two blocks, and under the two blocks)
    }

    texcoords[0].u = u / 16. + uvcorners[orientation][0];
    texcoords[0].v = (15 - v) / 16. + uvcorners[orientation][1];

    texcoords[1].u = u / 16. + uvcorners[orientation + 1][0];
    texcoords[1].v = (15 - v) / 16. + uvcorners[orientation + 1][1];

    texcoords[2].u = u / 16. + uvcorners[orientation + 2][0];
    texcoords[2].v = (15 - v) / 16. + uvcorners[orientation + 2][1];

    texcoords[3].u = u / 16. + uvcorners[orientation + 3][0];
    texcoords[3].v = (15 - v) / 16. + uvcorners[orientation + 3][1];

    return true;
}


bool wool_texfunc(unsigned short x, unsigned short y, unsigned short z,
                  struct Sector *sector, unsigned char face,
                  struct uv *texcoords, struct color *colors)
{
    float u, v;
    unsigned char data = sector->blockdata[x][z][y];

    switch (data)
    {
        case 0: // Normal (white)
            u = 0; v = 4; break;
        case 1: // Orange
            u = 2; v = 13; break;
        case 2: // Magenta
            u = 2; v = 12; break;
        case 3: // Light blue
            u = 2; v = 11; break;
        //TODO: the over types...
        default:
            u = 0; v = 4;
    }

    texcoords[0].u = u / 16. + uvcorners[0][0];
    texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

    texcoords[1].u = u / 16. + uvcorners[1][0];
    texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

    texcoords[2].u = u / 16. + uvcorners[2][0];
    texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

    texcoords[3].u = u / 16. + uvcorners[3][0];
    texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool bookshelf_texfunc(unsigned short x, unsigned short y, unsigned short z,
                       struct Sector *sector, unsigned char face,
                       struct uv *texcoords, struct color *colors)
{
    float u, v;

    //TODO: check
    if (face == BOTTOM || face == TOP)
    {
        u = blocktypes[5].texcoords.u;
        v = blocktypes[5].texcoords.v;
    }
    else
    {
        u = blocktypes[47].texcoords.u;
        v = blocktypes[47].texcoords.v;
    }

    texcoords[0].u = u / 16. + uvcorners[0][0];
    texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

    texcoords[1].u = u / 16. + uvcorners[1][0];
    texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

    texcoords[2].u = u / 16. + uvcorners[2][0];
    texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

    texcoords[3].u = u / 16. + uvcorners[3][0];
    texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool crafting_table_texfunc(unsigned short x, unsigned short y, unsigned short z,
                            struct Sector *sector, unsigned char face,
                            struct uv *texcoords, struct color *colors)
{
    float u, v;

    if (face == TOP)
    {
        u = 11;
        v = 2;
    }
    else if (face == BOTTOM)
    {
        u = blocktypes[5].texcoords.u;
        v = blocktypes[5].texcoords.v;
    }
    else
    {
        u = 11 + 0; //TODO: 11-12
        v = 3;
    }

    texcoords[0].u = u / 16. + uvcorners[0][0];
    texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

    texcoords[1].u = u / 16. + uvcorners[1][0];
    texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

    texcoords[2].u = u / 16. + uvcorners[2][0];
    texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

    texcoords[3].u = u / 16. + uvcorners[3][0];
    texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool seeds_texfunc(unsigned short x, unsigned short y, unsigned short z,
                   struct Sector *sector, unsigned char face,
                   struct uv *texcoords, struct color *colors)
{
    float u, v;

    if (face == TOP || face == BOTTOM)
    {
        return false;
    }
    else
    {
        u = 8 + (sector->blockdata[x][z][y] & 7);
        v = 5;
    }

    texcoords[0].u = u / 16. + uvcorners[0][0];
    texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

    texcoords[1].u = u / 16. + uvcorners[1][0];
    texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

    texcoords[2].u = u / 16. + uvcorners[2][0];
    texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

    texcoords[3].u = u / 16. + uvcorners[3][0];
    texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool pumpkin_texfunc(unsigned short x, unsigned short y, unsigned short z,
                     struct Sector *sector, unsigned char face,
                     struct uv *texcoords, struct color *colors)
{
    //TODO: check
    float u, v;
    unsigned char orientation_to_face[] = {EAST, SOUTH, WEST, NORTH};
    unsigned char type = sector->blocktypes[x][z][y];

    if (face == TOP)
    {
        u = 6;
        v = 6;
    }
    else if (face == orientation_to_face[sector->blockdata[x][z][y] & 5])
    {
        u = blocktypes[type].texcoords.u;
        v = blocktypes[type].texcoords.v;
    }
    else
    {
        u = 6;
        v = 7;
    }

    texcoords[0].u = u / 16. + uvcorners[0][0];
    texcoords[0].v = (15 - v) / 16. + uvcorners[0][1];

    texcoords[1].u = u / 16. + uvcorners[1][0];
    texcoords[1].v = (15 - v) / 16. + uvcorners[1][1];

    texcoords[2].u = u / 16. + uvcorners[2][0];
    texcoords[2].v = (15 - v) / 16. + uvcorners[2][1];

    texcoords[3].u = u / 16. + uvcorners[3][0];
    texcoords[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


struct BlockType blocktypes[256] = {
    [0] = {.name = "air",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT},
    [1] = {.name = "stone",
           .texcoords = {1, 0}},
    [2] = {.name = "grass",
           .texcoords = {3, 0},
           .texfunc = grass_texfunc},
    [3] = {.name = "dirt",
           .texcoords = {2, 0}},
    [4] = {.name = "cobblestone",
           .texcoords = {0, 1}},
    [5] = {.name = "wooden plank",
           .texcoords = {4, 0}},
    [6] = {.name = "sapling",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {15, 0},
           .texfunc = NULL}, //TODO
    [7] = {.name = "bedrock",
           .texcoords = {1, 1}},
    [8] = {.name = "water",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
           .texcoords = {13, 12}, //TODO: check
           .texfunc = NULL}, //TODO
    [9] = {.name = "stationary water",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {13, 12}}, //TODO: check
    [10] = {.name = "lava",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {13, 14}, //TODO: check
            .texfunc = NULL}, //TODO
    [11] = {.name = "stationary lava",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {13, 14}}, //TODO: check
    [12] = {.name = "sand",
            .texcoords = {2, 1}},
    [13] = {.name = "gravel",
            .texcoords = {3, 1}},
    [14] = {.name = "gold ore",
            .texcoords = {2, 0}},
    [15] = {.name = "iron ore",
            .texcoords = {2, 1}},
    [16] = {.name = "coal ore",
            .texcoords = {2, 2}},
    [17] = {.name = "wood",
            .texcoords = {4, 1},
            .texfunc = wood_texfunc}, //TODO: switch based on type
    [18] = {.name = "leaves",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {4, 3},
            .texfunc = leaves_texfunc}, //TODO: switch based on type and decay
    [20] = {.name = "glass",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {1, 3}},
    [21] = {.name = "lapis lazuli ore",
            .texcoords = {0, 10}},
    [22] = {.name = "lapis lazuli block",
            .texcoords = {0, 9}},
    [23] = {.name = "dispenser",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {14, 2},
            .texfunc = dispenser_texfunc},
    [24] = {.name = "sandstone",
            .texcoords = {0, 11}, //TODO
            .texfunc = NULL}, //TODO
    [25] = {.name = "note block",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {10, 3}},
    [26] = {.name = "bed",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE,
            .texcoords = {6, 8},
            .texfunc = bed_texfunc},
    [27] = {.name = "powered rail",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {3, 11},
            .texfunc = NULL}, // TODO
    [28] = {.name = "detector rail",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {3, 12},
            .texfunc = NULL}, // TODO
    [35] = {.name = "wool",
            .texcoords = {0, 4}, //TODO
            .texfunc = wool_texfunc}, //TODO: handle more colors
    [37] = {.name = "dandelion",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {13, 0},
            .texfunc = NULL}, //TODO: for orientation
    [38] = {.name = "rose",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {12, 0},
            .texfunc = NULL}, //TODO: for orientation
    [39] = {.name = "brown mushroom",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {13, 1},
            .texfunc = NULL}, //TODO: for orientation
    [40] = {.name = "red mushroom",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {12, 1},
            .texfunc = NULL}, //TODO: for orientation
    [41] = {.name = "gold block",
            .texcoords = {7, 1}},
    [42] = {.name = "iron block",
            .texcoords = {6, 1}},
    [43] = {.name = "double slabs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [44] = {.name = "slabs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [45] = {.name = "brick block",
            .texcoords = {7, 0}},
    [46] = {.name = "tnt",
            .texfunc = NULL}, //TODO
    [47] = {.name = "bookshelf",
            .texcoords = {3, 2},
            .texfunc = bookshelf_texfunc}, //TODO: check
    [48] = {.name = "moss stone",
            .texcoords = {4, 2}},
    [49] = {.name = "obsidian",
            .texcoords = {5, 2}},
    [50] = {.name = "torch",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [51] = {.name = "fire",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [52] = {.name = "monster spawner",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {1, 4}}, //TODO
    [53] = {.name = "wooden stairs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [54] = {.name = "chest",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {11, 1},
            .texfunc = NULL}, //TODO
    [55] = {.name = "redstone wire",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [56] = {.name = "diamond ore",
            .texcoords = {2, 3}},
    [57] = {.name = "diamond block",
            .texcoords = {8, 1}},
    [58] = {.name = "crafting table",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {11, 2},
            .texfunc = crafting_table_texfunc},
    [59] = {.name = "seeds",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {8, 5}, //TODO: use that in function
            .texfunc = seeds_texfunc},
    [60] = {.name = "farmland",
            .texcoords = {6, 5}, //TODO
            .texfunc = NULL}, //TODO
    [61] = {.name = "furnace",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {12, 2},
            .texfunc = dispenser_texfunc},
    [62] = {.name = "burning furnace",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {13, 3},
            .texfunc = dispenser_texfunc},
    [63] = {.name = "sign post",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [64] = {.name = "wooden door",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {1, 5},
            .texfunc = NULL}, //TODO
    [65] = {.name = "ladders",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [66] = {.name = "rails",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [67] = {.name = "cobblestone stairs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [68] = {.name = "wall sign",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [69] = {.name = "lever",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [70] = {.name = "stone pressure plate",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [71] = {.name = "iron door",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [72] = {.name = "wooden pressure plate",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [73] = {.name = "redstone ore",
            .texcoords = {3, 3}},
    [74] = {.name = "glowing redstone ore",
            .texcoords = {3, 3}}, //TODO
    [75] = {.name = "redstone torch (off)",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [76] = {.name = "redstone torch (on)",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [77] = {.name = "stone button",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [78] = {.name = "snow",
           .texcoords = {4, 4},
           .texfunc = grass_texfunc},
    [79] = {.name = "ice",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {3, 4}},
    [80] = {.name = "snow block",
            .texcoords = {2, 4}},
    [81] = {.name = "cactus",
            .texcoords = {5, 4},
            .texfunc = NULL}, //TODO: share with trees?
    [82] = {.name = "clay block",
            .texcoords = {8, 4}},
    [83] = {.name = "sugar cane",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {9, 4},
            .texfunc = NULL}, //TODO: share with flowers/mushrooms?
    [84] = {.name = "jukebox",
            .texcoords = {11, 4},
            .texfunc = NULL}, //TODO: share with bookshelf?
    [85] = {.name = "fence",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [86] = {.name = "pumpkin",
            .texcoords = {7, 7},
            .texfunc = pumpkin_texfunc},
    [87] = {.name = "netherrack",
            .texcoords = {7, 6}},
    [88] = {.name = "soul sand",
            .texcoords = {8, 6}},
    [89] = {.name = "glowstone block",
            .texcoords = {9, 6}},
    [90] = {.name = "portal",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [91] = {.name = "jack-o-lantern",
            .texcoords = {8, 7},
            .texfunc = pumpkin_texfunc},
    [92] = {.name = "cake block",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE,
            .texfunc = NULL}, //TODO
    [93] = {.name = "redstone repeater (off)",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK, //TODO: check
            .texfunc = NULL}, //TODO
    [94] = {.name = "redstone repeater (on)",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK, //TODO: check
            .texfunc = NULL}, //TODO
    //... TODO...
};
