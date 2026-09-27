#include "../include/vector.h"

Vector3 scale(const Vector3& v, float scalar) {
    return Vector3{v.x * scalar, v.y * scalar, v.z * scalar};
}

float dot_prod(const Vector3& u, const Vector3& v) {
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

// returns projection of u onto v
Vector3 proj_vec(const Vector3& u, const Vector3& v) {
    float len_v_sqrd = dot_prod(v, v);
        if (len_v_sqrd == 0) return Vector3{0, 0, 0};

    float dot = dot_prod(u, v);
    float scalar = dot/len_v_sqrd;

    return Vector3{v.x * scalar, v.y * scalar, v.z * scalar};
}
