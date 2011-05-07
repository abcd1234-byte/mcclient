#include "sector.h"


short _faces_count[] = {0, 1, 1, 2, 1, 2, 2, 3};


//TODO
void octree_update_face_count(short idx, short delta, short size, Octree octree, short x, short y, short z)
{
    short dx, dy, dz;

    if (size == 1)
        return;

    size /= 2;

    octree[idx] += delta;

    dx = (x < size) ? 0 : 1;
    dy = (y < size) ? 0 : 1;
    dz = (z < size) ? 0 : 1;

    octree_update_face_count(OCTREE_GET_CHILD(idx, dx, dy, dz), delta, size, octree,
                             x - dx * size, y - dy * size, z - dz * size);
}



// Sector-related functions:
struct Sector *sector_new(int cx, int cz)
{
    struct Sector* sector = calloc(1, sizeof(struct Sector));
    if (sector == NULL)
        return NULL;

    sector->cx = cx;
    sector->cz = cz;

    return sector;
}


void sector_set_chunk(struct Sector *sector,
                      short ox, short oy, short oz,
                      short size_x, short size_y, short size_z,
                      const unsigned char *data)
{
    // TODO: optimize
    const unsigned char *metadata = data + (size_x * size_y * size_z);
    const unsigned char *lighting = data + (size_x * size_y * size_z) * 3 / 2;
    for (unsigned short x=0; x < size_x; x++)
    {
        for (unsigned short z=0; z < size_z; z++)
        {
            memcpy(sector->blocktypes[ox + x][oz + z] + oy,
                   data + (z * size_y) + (x * size_z * size_y),
                   size_y);
            for (unsigned short y=0; y < size_y; y++)
            {
                unsigned int index = (z * size_y) + (x * size_z * size_y) + y;
                if (index & 1)
                {
                    sector->blockdata[ox + x][oz + z][oy + y] = metadata[index / 2] >> 4;
                    sector->lighting[ox + x][oz + z][oy + y] = lighting[index / 2] >> 4;
                }
                else
                {
                    sector->blockdata[ox + x][oz + z][oy + y] = metadata[index / 2] & 15;
                    sector->lighting[ox + x][oz + z][oy + y] = lighting[index / 2] & 15;
                }
            }
        }
    }
}


static inline void _sector_compute_faces(struct Sector *sector, short x, short y, short z)
{
    sector->blockfaces[x][z][y] = 0;
    if (sector->blocktypes[x][z][y] != 0)
    {
        // TODO: transparent VS non-transparent
        if (x == 15 || sector->blocktypes[x + 1][z][y] == 0)// || BlockTypes[sector->blocktypes[x + 1][z][y]].transparent)
            sector->blockfaces[x][z][y] |= FACE_SOUTH;
        if (y == 128 || sector->blocktypes[x][z][y + 1] == 0)// || BlockTypes[sector->blocktypes[x][z][y + 1]].transparent)
            sector->blockfaces[x][z][y] |= FACE_TOP;
        if (z == 15 || sector->blocktypes[x][z + 1][y] == 0)// || BlockTypes[sector->blocktypes[x][z + 1][y]].transparent)
            sector->blockfaces[x][z][y] |= FACE_WEST;
        if (x == 0 || sector->blocktypes[x - 1][z][y] == 0)// || BlockTypes[sector->blocktypes[x - 1][z][y]].transparent)
            sector->blockfaces[x][z][y] |= FACE_NORTH;
        if (y == 0 || sector->blocktypes[x][z][y - 1] == 0)// || BlockTypes[sector->blocktypes[x][z][y - 1]].transparent)
            sector->blockfaces[x][z][y] |= FACE_BOTTOM;
        if (z == 0 || sector->blocktypes[x][z - 1][y] == 0)// || BlockTypes[sector->blocktypes[x][z - 1][y]].transparent)
            sector->blockfaces[x][z][y] |= FACE_EAST;
    }
}

static inline void sector_update_block_faces(struct Sector *sector, short x, short y, short z)
{
    short old_count, new_count;

    old_count = _faces_count[sector->blockfaces[x][z][y] >> 3] + _faces_count[sector->blockfaces[x][z][y] & 7];
    _sector_compute_faces(sector, x, y, z);
    new_count = _faces_count[sector->blockfaces[x][z][y] >> 3] + _faces_count[sector->blockfaces[x][z][y] & 7];

    octree_update_face_count(0, new_count - old_count, 16, sector->octrees[y / 16], x, y % 16, z);
}

void sector_change_block_type(struct Sector *sector, short x, short y, short z, char blocktype)
{
    sector->blocktypes[x][z][y] = blocktype;

    // Update faces
    sector_update_block_faces(sector, x, y, z);
    if (x > 0)
        sector_update_block_faces(sector, x - 1, y, z);
    if (y > 0)
        sector_update_block_faces(sector, x, y - 1, z);
    if (z > 0)
        sector_update_block_faces(sector, x, y, z - 1);
    if (x < 15)
        sector_update_block_faces(sector, x + 1, y, z);
    if (y < 127)
        sector_update_block_faces(sector, x, y + 1, z);
    if (z < 15)
        sector_update_block_faces(sector, x, y, z + 1);
}

void sector_gen_faces(struct Sector *sector)
{
    memset(sector->octrees, 0, 16 * (1 + 8 + 64 + 512));

    for(unsigned short x=0; x < 16; x++)
    {
        for (unsigned short y=0; y < 128; y++)
        {
            for (unsigned short z=0; z < 16; z++)
            {
                _sector_compute_faces(sector, x, y, z);
                octree_update_face_count(0, _faces_count[sector->blockfaces[x][z][y] >> 3] + _faces_count[sector->blockfaces[x][z][y] & 7], 16, sector->octrees[y / 16], x, y % 16, z);
            }
        }
    }
}

unsigned int sector_count_faces(struct Sector *sector)
{
    unsigned int count = 0;
    for (unsigned short i=0; i < 16; i++)
        count += sector->octrees[i][0];
    return count;
}


void get_sector_coords(int *x, int *y, int *z, int *cx, int *cz)
{
    *cx = (*x) >> 4;
    *cz = (*z) >> 4;
    *x = (*x) & 15;
    *z = (*z) & 15;
}


int main(void)
{
    struct Sector *sector = sector_new(0, 0);
    unsigned int idx;
    unsigned char truc = 0;

    for(unsigned short x=0; x < 16; x++)
        for (unsigned short y=0; y < 128; y++)
            for (unsigned short z=0; z < 16; z++)
                sector->blocktypes[x][z][y] = rand() % 2;

    for (int i=0; i < 420; i++)
    {
        sector_gen_faces(sector);
    }

    for (int i=0; i < 42000; i++)
        sector_change_block_type(sector, rand() % 16, rand() % 128, rand() % 16, rand() % 2);


    idx = 0;
    printf("%d\n", sector->octrees[0][idx]);
    idx = OCTREE_GET_CHILD(idx, 0, 0, 0);
    printf("%d\n", sector->octrees[0][idx]);
    idx = OCTREE_GET_CHILD(idx, 0, 0, 0);
    printf("%d\n", sector->octrees[0][idx]);

    truc = FACE_SOUTH | FACE_EAST | FACE_NORTH | FACE_TOP | FACE_BOTTOM;
    printf("%d; %d + %d\n", truc, _faces_count[truc >> 3], _faces_count[truc & 7]);
}
