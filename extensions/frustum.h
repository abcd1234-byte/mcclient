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

#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

struct Plane {
    float a, b, c, d;
};

struct Vec3D {
    float x, y, z;
};

struct Vec3D vec3d_add(struct Vec3D v1, struct Vec3D v2);
struct Vec3D vec3d_addmul(struct Vec3D v1, struct Vec3D v2, double coeff);
struct Vec3D vec3d_neg(struct Vec3D v1);
struct Vec3D vec3d_cross_product(struct Vec3D v1, struct Vec3D v2);
char wrong_side_of_plane(struct Plane plane, struct Vec3D vec);
char right_side_of_plane(struct Plane plane, struct Vec3D vec);
struct Plane plane_from_normal_and_point(struct Vec3D normal, struct Vec3D point);
void make_frustum(double fov, double ratio, double znear, double zfar,
                                struct Vec3D pos, double yaw, double pitch, struct Plane *frustum);

#endif
