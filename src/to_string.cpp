#include "../include/to_string.h"

std::string float_to_dec(float val) {
    std::stringstream ss;
    ss << std::setprecision(6) << val;
    return ss.str();
}

std::string nvec_to_string(float x, float y, float z) {
    std::string s1 = "x", s2 = "y";
    if (y>=0) s1 = "x+";
    if (z>=0) s2 = "y+";
    return float_to_dec(x) + s1 + float_to_dec(y) + s2 + float_to_dec(z) + "z = 0";
}