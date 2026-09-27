#pragma once

#include "vector.h"
#include "to_string.h"

const std::vector<sf::Color> plane_colors = {
    sf::Color::Red,
    sf::Color::Blue,
    sf::Color::Magenta
};

struct Plane {
    Vector3 n;
    sf::Color color;
    std::string text;

    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
    Vector3 p4;

    Plane(const Vector3& n, sf::Color c) : color(c), n(n) {
        Vector3 a;
        float ax = std::abs(n.x), ay = std::abs(n.y), az = std::abs(n.z);
        if (ax <= ay && ax <= az)      a = {1, 0, 0};
        else if (ay <= az)             a = {0, 1, 0};
        else                           a = {0, 0, 1};
    
        Vector3 v1 = Vector3{n.y * a.z - n.z * a.y,
                             n.z * a.x - n.x * a.z,
                             n.x * a.y - n.y * a.x};
        
        Vector3 v2 = Vector3{n.y * v1.z - n.z * v1.y,
                             n.z * v1.x - n.x * v1.z,
                             n.x * v1.y - n.y * v1.x};
        
        p1 = scale(v1, 5/v1.length());
        p2 = scale(v2, 5/v2.length());
        p3 = scale(v1, -5/v1.length());
        p4 = scale(v2, -5/v2.length());

        text = nvec_to_string(n.x, n.y, n.z);
    }

    Plane(const Vector3& u1, const Vector3& u2, sf::Color c) : color(c) {
        Vector3 v1 = u1;

        Vector3 v2 = u2 - proj_vec(u2, u1);

        n = Vector3{v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x};

        p1 = scale(v1, 5/v1.length());
        p2 = scale(v2, 5/v2.length());
        p3 = scale(v1, -5/v1.length());
        p4 = scale(v2, -5/v2.length());

        text = nvec_to_string(n.x, n.y, n.z);
    }

};

Vector3 proj_plane(const Vector3& u, const Plane& p);