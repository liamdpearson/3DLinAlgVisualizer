#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <cmath>

#include "to_string.h"

const std::vector<sf::Color> vector_colors = {
    sf::Color::Cyan,
    sf::Color::Magenta,
    sf::Color::Green,
    sf::Color::Yellow,
    sf::Color::Red,
    sf::Color::Blue
};

struct Vector3 {
    float x;
    float y;
    float z;
    
    Vector3 operator+(const Vector3& other) const {
        return Vector3{x + other.x, y + other.y, z + other.z};
    }
    Vector3 operator-(const Vector3& other) const {
        return Vector3{x - other.x, y - other.y, z - other.z};
    }
    bool operator==(const Vector3& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }
    float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    void normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
            z /= len;
        }
    }
    void scale(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }
};

struct Vector {
    Vector3 vec;
    sf::Color color;
    std::string text = "(" + float_to_dec(vec.x) + ", " + float_to_dec(vec.y) + ", " + float_to_dec(vec.z) + ")";
};

struct BasisVector {
    Vector3 vec;
    std::string text;
};

Vector3 scale(const Vector3& v, float scalar);

float dot_prod(const Vector3& u, const Vector3& v);

Vector3 proj_vec(const Vector3& u, const Vector3& v);