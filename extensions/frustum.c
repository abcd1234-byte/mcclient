#include "frustum.h"
#include <math.h>

inline struct Vec3D vec3d_add(struct Vec3D v1, struct Vec3D v2)
{
    struct Vec3D v3 = {.x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z};
    return v3;
}


inline struct Vec3D vec3d_neg(struct Vec3D v1)
{
    struct Vec3D v3 = {.x = -v1.x, .y = -v1.y, .z = - v1.z};
    return v3;
}


inline struct Vec3D vec3d_addmul(struct Vec3D v1, struct Vec3D v2, double coeff)
{
    struct Vec3D v3 = {.x = v1.x + v2.x * coeff, .y = v1.y + v2.y * coeff, .z = v1.z + v2.z * coeff};
    return v3;
}


inline struct Vec3D vec3d_cross_product(struct Vec3D v1, struct Vec3D v2)
{
    struct Vec3D v3 = {.x = v1.y * v2.z - v1.z * v2.y,
                       .y = v1.z * v2.x - v1.x * v2.z,
                       .z = v1.x * v2.y - v1.y * v2.x};
    return v3;
}

inline char wrong_side_of_plane(struct Plane plane, struct Vec3D vec)
{
    return (vec.x * plane.a + vec.y * plane.b + vec.z * plane.c + plane.d) < 0;
}

inline struct Plane plane_from_normal_and_point(struct Vec3D normal, struct Vec3D point)
{
    struct Plane plane;
    plane.a = normal.x;
    plane.b = normal.y;
    plane.c = normal.z;
    plane.d = - (plane.a * point.x + plane.b * point.y + plane.c * point.z);
    return plane;
}

extern inline void make_frustum(double fov, double ratio, double znear, double zfar,
                                struct Vec3D pos, double yaw, double pitch, struct Plane *frustum)
{
    struct Vec3D up, direction, right;
    struct Vec3D nc, fc, aux;
    double near_h, near_w;

    near_h = tan(fov / 2) * znear;
    near_w = ratio * near_h;

    // TODO: triple-check
    direction.x = - cos(pitch) * sin(yaw);
    direction.y = - sin(pitch);
    direction.z = cos(pitch) * cos(yaw);

    // TODO: quadruple-check
    up.x = - sin(pitch) * sin(yaw);
    up.y = cos(pitch);
    up.z = sin(pitch) * cos(yaw);

    // Compute some points and stuff
    nc = vec3d_addmul(pos, direction, znear);
    fc = vec3d_addmul(pos, direction, zfar);
    right = vec3d_cross_product(direction, up);

    // Near plane
    frustum[0] = plane_from_normal_and_point(direction, nc);

    // Far plane
    frustum[1] = plane_from_normal_and_point(vec3d_neg(direction), fc);

    // Right plane
    aux = vec3d_addmul(nc, right, near_w);
    aux = vec3d_addmul(aux, pos, - 1);
    aux = vec3d_cross_product(up, aux);
    frustum[2] = plane_from_normal_and_point(aux, pos);

    // Left plane
    aux = vec3d_addmul(nc, right, -near_w);
    aux = vec3d_addmul(aux, pos, - 1);
    aux = vec3d_cross_product(aux, up);
    frustum[3] = plane_from_normal_and_point(aux, pos);

    // Top plane
    aux = vec3d_addmul(nc, up, near_h);
    aux = vec3d_addmul(aux, pos, - 1);
    aux = vec3d_cross_product(aux, right);
    frustum[4] = plane_from_normal_and_point(aux, pos);

    // Bottom plane
    aux = vec3d_addmul(nc, up, -near_h);
    aux = vec3d_addmul(aux, pos, - 1);
    aux = vec3d_cross_product(right, aux);
    frustum[5] = plane_from_normal_and_point(aux, pos);

}
