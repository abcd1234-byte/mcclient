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
struct Plane plane_from_normal_and_point(struct Vec3D normal, struct Vec3D point);
void make_frustum(double fov, double ratio, double znear, double zfar,
                                struct Vec3D pos, double yaw, double pitch, struct Plane *frustum);

#endif
