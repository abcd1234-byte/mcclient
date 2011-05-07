#ifndef __OCTREE_H__
#define __OCTREE_H__

#define     OCTREE_GET_CHILD(idx, dx, dy, dz) \
                ((idx) * 8 + 1 + 4 * (dx) + 2 * (dz) + (dy))
#define     OCTREE_GET_PARENT(idx) (((idx) - 1) / 8)

#endif
