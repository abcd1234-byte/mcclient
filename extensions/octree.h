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

#ifndef __OCTREE_H__
#define __OCTREE_H__

#define     OCTREE_GET_CHILD(idx, dx, dy, dz) \
                ((idx) * 8 + 1 + 4 * (dx) + 2 * (dz) + (dy))
#define     OCTREE_GET_PARENT(idx) (((idx) - 1) / 8)

#endif
