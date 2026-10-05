#include "../include/plane.h"

Vector3 proj_plane(const Vector3& u, const Plane& p) {
    Vector3 proj_onto_n = proj_vec(u, p.n);
    return Vector3{u.x - proj_onto_n.x, u.y - proj_onto_n.y, u.z - proj_onto_n.z};
}