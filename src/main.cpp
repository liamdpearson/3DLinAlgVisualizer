#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include "../include/to_string.h"
#include "../include/plane.h"
#include "../include/vector.h"
#include "../include/Arial_ttf.h"


const int SCREEN_WIDTH = 1000, SCREEN_HEIGHT = 1000;

std::tuple<float, float, float> calc_dist(const Vector3 vec, Vector3 cam_vec, float cam_len_sqrd, Vector3 y_vec, Vector3 x_vec) {
    Vector3 proj_onto_cam = proj_vec(vec, cam_vec);
    Vector3 perp_onto_cam = vec - proj_onto_cam;

    // scalar component of vec along cam_vec direction
    float along = dot_prod(vec, cam_vec) / cam_len_sqrd;

    float z_dist;
    if (along >= 1.0f) {
        z_dist = 0.001f;  // behind camera, push to near-zero
    } else {
        z_dist = (1.0f - along) * cam_vec.length();
    }

    float x_dist = dot_prod(perp_onto_cam, x_vec) / x_vec.length();
    float y_dist = dot_prod(perp_onto_cam, y_vec) / y_vec.length();

    return {x_dist, y_dist, z_dist};
}

std::vector<std::string> split_by_spaces(std::string s) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

sf::Color choose_vec_color(const std::vector<Vector>& vectors) {
    for (sf::Color color : vector_colors) {
        bool used = false;
        for (const Vector& vec : vectors) {
            if (vec.color == color) {
                used = true;
                break;
            }
        }
        if (!used) return color;
    }
    return sf::Color::White;  // fallback
}

sf::Color choose_plane_color(const std::vector<Plane>& planes) {
    for (sf::Color color : plane_colors) {
        bool used = false;
        for (const Plane& plane : planes) {
            if (plane.color == color) {
                used = true;
                break;
            }
        }
        if (!used) return color;
    }
    return sf::Color::White; // fallback
}

void handle_cmd_input(std::vector<Vector>& vectors, std::vector<Plane>& planes, const std::string cmd, std::string& e_msg, std::string& help_msg) {
    std::vector<std::string> tokens = split_by_spaces(cmd);
    if (tokens.empty()) {
        e_msg = "Error: no command entered";
        return;
    }
    help_msg = "";
    if (tokens[0] == "new") {
        if (tokens.size() > 1) {
            if (tokens[1] == "vec" || tokens[1] == "vector") {

                if (vectors.size() >= 6) {
                    e_msg = "Error: too many vectors";
                }
                else if (tokens.size() == 5) {
                    try {
                        float x = std::stof(tokens[2]);
                        float y = std::stof(tokens[3]);
                        float z = std::stof(tokens[4]);
                        vectors.push_back(Vector{Vector3{x, y, z}, choose_vec_color(vectors)});
                        e_msg = "";
                    } catch (std::exception e) {
                        e_msg = "Error: invalid vector value input";
                    }
                }
                else {
                    e_msg = "Error: invalid amount of arguments";
                }
            }

            else if (tokens[1] == "plane") {

                if (planes.size() >= 3) {
                    e_msg = "Error: too many planes";
                }
                else if (tokens.size() == 3 && tokens[2].length() == 2) {
                    if (tokens[2][0] == 'v') {
                        if (std::isdigit(tokens[2][1])) {
                            int n = tokens[2][1] - '0';
                            if (n < 1 || n > vectors.size()) {
                                e_msg = "Error: index out of range";
                                return;
                            }
                            Vector3 normal = vectors[n-1].vec;
                            if (normal.length() <= 1e-6f) {
                                e_msg = "Error: cannot create plane with zero normal vector";
                                return;
                            }
                            planes.push_back(Plane{normal, choose_plane_color(planes)});
                            e_msg = "";
                        } else {
                            e_msg = "Error: invalid input";
                        }
                    }
                    else {
                        e_msg = "Error: invalid input";
                    }
                }
                else if (tokens.size() == 4) {
                    if (tokens[2][0] == 'v' && tokens[2].length() == 2 && tokens[3][0] == 'v' && tokens[3].length() == 2) {
                        if (std::isdigit(tokens[2][1]) && std::isdigit(tokens[3][1])) {
                            int n1 = tokens[2][1] - '0';
                            int n2 = tokens[3][1] - '0';
                            if (n1 < 1 || n1 > vectors.size() || n2 < 1 || n2 > vectors.size()) {
                                e_msg = "Error: index out of range";
                                return;
                            }
                            Vector3 v1 = vectors[n1-1].vec;
                            Vector3 v2 = vectors[n2-1].vec;
                            if (v1.length() <= 1e-6f || v2.length() <= 1e-6f) {
                                e_msg = "Error: cannot create plane with zero vector";
                                return;
                            }
                            planes.push_back(Plane{v1, v2, choose_plane_color(planes)});
                            e_msg = "";
                        } else {
                            e_msg = "Error: invalid input";
                        }
                    }
                    else {
                        e_msg = "Error: invalid input";
                    }
                }
                else {
                    e_msg = "Error: invalid amount of arguments";
                }
            }
            else {
                e_msg = "Error: unrecognized object";
            }
        }
        else {
            e_msg = "Error: invalid amount of arguments";
        } 
    }
    else if (tokens[0] == "add" || tokens[0] == "sub") {
        if (tokens.size() == 3) {
            int a, b;
            if (tokens[1][0] == 'v' && tokens[1].length() == 2 && tokens[2][0] == 'v' && tokens[2].length() == 2) {
                if (std::isdigit(tokens[1][1]) && std::isdigit(tokens[2][1])) {
                    a = tokens[1][1] - '0';
                    if (a < 1 || a > vectors.size()) {
                        e_msg = "Error: index out of range";
                        return;
                    }
                    b = tokens[2][1] - '0';
                    if (b < 1 || b > vectors.size()) {
                        e_msg = "Error: index out of range";
                        return;
                    }
                } else {
                    e_msg = "Error: invalid input";
                    return;
                }
            } else {
                e_msg = "Error: invalid input";
                return;
            }

            if (tokens[0] == "add") {
                vectors[b-1].vec.x += vectors[a-1].vec.x;
                vectors[b-1].vec.y += vectors[a-1].vec.y;
                vectors[b-1].vec.z += vectors[a-1].vec.z;
            }
            if (tokens[0] == "sub") {
                vectors[b-1].vec.x -= vectors[a-1].vec.x;
                vectors[b-1].vec.y -= vectors[a-1].vec.y;
                vectors[b-1].vec.z -= vectors[a-1].vec.z;
            }
            vectors[b-1].text = "(" + float_to_dec(vectors[b-1].vec.x) + ", " + float_to_dec(vectors[b-1].vec.y) + ", " + float_to_dec(vectors[b-1].vec.z) + ")";
            e_msg = "";
        }
        else {
            e_msg = "Error: invalid amount of arguments";
        } 
    }
    else if (tokens[0] == "cross") {
        if (vectors.size() < 6) {
            if (tokens.size() == 3) {
                int a, b;
                if (tokens[1][0] == 'v' && tokens[1].length() == 2 && tokens[2][0] == 'v' && tokens[2].length() == 2) {
                    if (std::isdigit(tokens[1][1]) && std::isdigit(tokens[2][1])) {
                        a = tokens[1][1] - '0';
                        if (a < 1 || a > vectors.size()) {
                            e_msg = "Error: index out of range";
                            return;
                        }
                        b = tokens[2][1] - '0';
                        if (b < 1 || b > vectors.size()) {
                            e_msg = "Error: index out of range";
                            return;
                        }
                    } else {
                        e_msg = "Error: invalid input";
                        return;
                    }
                } else {
                    e_msg = "Error: invalid input";
                    return;
                }

                float x = vectors[a-1].vec.y * vectors[b-1].vec.z - vectors[a-1].vec.z * vectors[b-1].vec.y;
                float y = vectors[a-1].vec.z * vectors[b-1].vec.x - vectors[a-1].vec.x * vectors[b-1].vec.z;
                float z = vectors[a-1].vec.x * vectors[b-1].vec.y - vectors[a-1].vec.y * vectors[b-1].vec.x;

                vectors.push_back(Vector{Vector3{x, y, z}, choose_vec_color(vectors)});
                e_msg = "";
            }
            else {
                e_msg = "Error: invalid amount of arguments";
            }
        }
        else {
            e_msg = "Error: too many vectors";
        }
    }
    else if (tokens[0] == "proj") {
        if (tokens.size() == 3) {
            if (vectors.size() >= 6) {
                e_msg = "Error: too many vectors";
                return;
            }
            int a, b;
            if (tokens[1][0] == 'v' && tokens[1].length() == 2 && tokens[2].length() == 2) {
                if (std::isdigit(tokens[1][1]) && std::isdigit(tokens[2][1])) {
                        a = tokens[1][1] - '0';
                        b = tokens[2][1] - '0';
                        if (a < 1 || a > vectors.size()) {
                            e_msg = "Error: index out of range";
                            return;
                        }
                        if (tokens[2][0] == 'v') {
                            if (b < 1 || b > vectors.size()) {
                                e_msg = "Error: index out of range";
                                return;
                            }
                            Vector3 vec = proj_vec(vectors[a-1].vec, vectors[b-1].vec);
                            vectors.push_back(Vector{vec, choose_vec_color(vectors)});
                            e_msg = "";
                        }
                        else if (tokens[2][0] == 'p') {
                            if (b < 1 || b > planes.size()) {
                                e_msg = "Error: index out of range";
                                return;
                            }
                            Vector3 vec = proj_plane(vectors[a-1].vec, planes[b-1]);
                            vectors.push_back(Vector{vec, choose_vec_color(vectors)});
                            e_msg = "";
                        }
                        else {
                            e_msg = "Error: invalid input";
                        }
                } else {
                    e_msg = "Error: invalid input";
                }
            } else {
                e_msg = "Error: invalid input";
            }
        }
        else {
            e_msg = "Error: invalid amount of arguments";
        }
    }
    else if (tokens[0] == "normalize") {
        if (tokens.size() == 2) {
            int a;
            if (tokens[1][0] == 'v' && tokens[1].length() == 2) {
                if (std::isdigit(tokens[1][1])) {
                        a = tokens[1][1] - '0';
                        if (a < 1 || a > vectors.size()) {
                            e_msg = "Error: index out of range";
                            return;
                        }
                } else {
                    e_msg = "Error: invalid input";
                    return;
                }
            } else {
                e_msg = "Error: invalid input";
                return;
            }
            if (vectors[a-1].vec.length() >= -1e-6f && vectors[a-1].vec.length() <= 1e-6f) {
                e_msg = "Error: cannot normalize zero vector";
                return;
            }
            if (vectors[a-1].vec.length() >= .999999f && vectors[a-1].vec.length() <= 1.000001f) {
                e_msg = "Error: already normalized";
                return;
            }

            float x = vectors[a-1].vec.x;
            float y = vectors[a-1].vec.y;
            float z = vectors[a-1].vec.z;

            float len_sqrd = x*x + y*y + z*z;
            
            vectors[a-1].vec.normalize();
            vectors[a-1].text = "(" + float_to_dec(x/len_sqrd) + ", " + float_to_dec(y/len_sqrd) + ", " + float_to_dec(z/len_sqrd) + ")";
            e_msg = "";
        }
        else {
            e_msg = "Error: invalid amount of arguments";
        }
    }
    else if (tokens[0] == "scale") {
        if (tokens.size() == 3) {
            int a;
            float scalar;
            if (tokens[1][0] == 'v' && tokens[1].length() == 2) {
                if (std::isdigit(tokens[1][1])) {
                    a = tokens[1][1] - '0';
                    if (a < 1 || a > vectors.size()) {
                        e_msg = "Error: index out of range";
                        return;
                    }
                } else {
                    e_msg = "Error: invalid input";
                    return;
                }
            } else {
                e_msg = "Error: invalid input";
                return;
            }
            try {
                scalar = std::stof(tokens[2]);
                if (scalar == 1) {
                    e_msg = "Error: don't waste my time";
                    return;
                }
            } catch (...) {
                e_msg = "Error: invalid scalar input";
                return;
            }
            vectors[a-1].vec.scale(scalar);
            vectors[a-1].text = "(" + float_to_dec(vectors[a-1].vec.x) + ", " + float_to_dec(vectors[a-1].vec.y) + ", " + float_to_dec(vectors[a-1].vec.z) + ")";
            e_msg = "";
        }
        else {
            e_msg = "Error: invalid amount of arguments";
        }
    }
    else if (tokens[0] == "del") {
        if (tokens.size() == 2) {
            if (tokens[1][0] == 'v' && tokens[1].length() == 2) {
                if (std::isdigit(tokens[1][1])) {
                    int i = tokens[1][1] - '0';

                    if (i < 1 || i > vectors.size()) {
                        e_msg = "Error: index out of range";
                        return;
                    }
                    vectors.erase(vectors.begin() + i-1);
                }
                else {
                    e_msg = "Error: invalid argument";
                }
            }
            else if (tokens[1][0] == 'p' && tokens[1].length() == 2) {
                if (std::isdigit(tokens[1][1])) {
                    int i = tokens[1][1] - '0';

                    if (i < 1 || i > planes.size()) {
                        e_msg = "Error: index out of range";
                        return;
                    }
                    planes.erase(planes.begin() + i-1);
                }
                else {
                    e_msg = "Error: invalid argument";
                }
            }   
            else {
                e_msg = "Error: invalid argument";
            }
        }
        else {
            e_msg = "Error: invalid amount of arguments";
        }
    }
    else if (tokens[0] == "clear" && tokens.size() == 1) {
        e_msg = "";
        vectors = {};
        planes = {};
    }
    else if (tokens[0] == "help" && tokens.size() == 1) {
        help_msg =  std::string("Create New:\n")
                   + "  Vector: 'new vec x y z'\n"
                   + "  Plane: 'new plane vN' / 'new plane vN vM'\n"
                   + "Operations:\n"
                   + "  'add vN vM' adds vN to vM\n"
                   + "  'sub vN vM' subtracts vN from vM\n"
                   + "  'cross vN vM' crossproduct\n"
                   + "Projections:\n"
                   + "  'proj vN vM' projects vN onto vM\n"
                   + "  'proj vN pN' projects vN onto pN\n"
                   + "Scaling: 'scale vN' scalar\n"
                   + "Normalization: 'normalize vN'\n"
                   + "Delete: 'del vN' / 'del pN'\n"
                   + "Clear All: 'clear'\n"
                   + "Note: vN and pN refer to the Nth vector and Nth plane, respectively.";
        e_msg = "";
    }
    else {
        e_msg = "Error: unrecognized command";
    }
}

int main() {
    
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "3DVectorSim");

    sf::Font font;
    font.loadFromMemory(Arial_ttf, Arial_ttf_len);
    std::string cur_command = "";
    std::string e_msg = "";
    std::string help_msg = "";

    float cam_yaw = 45.0f;
    float cam_pitch = 15.0f;
    float cam_dist = 10.0f;
    Vector3 up_vec = Vector3{0.0f, 0.0f, 1.0f};

    std::vector<BasisVector> basis_vectors;
    for (int i = 0; i < 4; ++i) {
        std::string pos = std::to_string(i + 1);
        std::string neg = "-" + pos;
        float f = (float)(i+1);
        basis_vectors.push_back(BasisVector{Vector3{ f, 0, 0}, pos});
        basis_vectors.push_back(BasisVector{Vector3{-f, 0, 0}, neg});
        basis_vectors.push_back(BasisVector{Vector3{0,  f, 0}, pos});
        basis_vectors.push_back(BasisVector{Vector3{0, -f, 0}, neg});
        basis_vectors.push_back(BasisVector{Vector3{0, 0,  f}, pos});
        basis_vectors.push_back(BasisVector{Vector3{0, 0, -f}, neg});
    }

    basis_vectors.push_back(BasisVector{Vector3{5, 0, 0}, "5\tX"});
    basis_vectors.push_back(BasisVector{Vector3{0, 5, 0}, "5\tY"});
    basis_vectors.push_back(BasisVector{Vector3{0, 0, 5}, "5\tZ"});
    basis_vectors.push_back(BasisVector{Vector3{-5, 0, 0}, "-5\tX"});
    basis_vectors.push_back(BasisVector{Vector3{0, -5, 0}, "-5\tY"});
    basis_vectors.push_back(BasisVector{Vector3{0, 0, -5}, "-5\tZ"});

    std::vector<Vector> vectors = {};
    std::vector<Plane> planes {};

    sf::Vector2i prev_mouse_pos = sf::Mouse::getPosition(window);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    float delta = event.mouseWheelScroll.delta * 0.1;

                    cam_dist -= delta;

                    if (cam_dist < 0.1) cam_dist = 0.1;
                }
            }

            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\r') {
                    if (!cur_command.empty()) {
                        handle_cmd_input(vectors, planes, cur_command, e_msg, help_msg);
                        cur_command = "";
                    }
                } 
                else if (event.text.unicode == '\b') {
                    if (!cur_command.empty()) cur_command.pop_back();
                } 
                else if (event.text.unicode < 128) {
                    cur_command += static_cast<char>(event.text.unicode);
                }
            }

            else if (event.type == sf::Event::Resized) {
                unsigned int newSize = std::max(event.size.width, event.size.height);
                window.setSize(sf::Vector2u(newSize, newSize));
            }
        }

        sf::Vector2i cur_mouse_pos = sf::Mouse::getPosition(window);
        sf::Vector2i delta = cur_mouse_pos - prev_mouse_pos;
        prev_mouse_pos = cur_mouse_pos;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            cam_yaw += delta.x;
            cam_pitch += delta.y;
            if (cam_pitch > 89.99) cam_pitch = 89.99;
            if (cam_pitch < -89.99) cam_pitch = -89.99;
        }
    
        float z = cam_dist * sin(cam_pitch*M_PI/180);

        float flat_dist = cam_dist * cos(cam_pitch*M_PI/180);
        float x = flat_dist * cos(cam_yaw*M_PI/180);
        float y = flat_dist * sin(cam_yaw*M_PI/180);
        

        Vector3 cam_vec = Vector3{x, y, z};
        float cam_len_sqrd = dot_prod(cam_vec, cam_vec);
        Vector3 y_vec = up_vec - proj_vec(up_vec, cam_vec);
        Vector3 x_vec = Vector3{
            cam_vec.y * y_vec.z - cam_vec.z * y_vec.y,
            cam_vec.z * y_vec.x - cam_vec.x * y_vec.z,
            cam_vec.x * y_vec.y - cam_vec.y * y_vec.x
        };

        // start drawing
        window.clear();

        sf::Text plane_header("Planes:", font, 20);
        plane_header.setPosition(sf::Vector2f(15, 65+50*(vectors.size())));
        window.draw(plane_header);

        for (int i = 0; i < planes.size(); i++) {
            Plane plane = planes[i];

            std::tuple<float, float, float> p1 = calc_dist(plane.p1, cam_vec, cam_len_sqrd, y_vec, x_vec);
            float p1x = std::get<0>(p1), p1y = std::get<1>(p1), p1z = std::get<2>(p1);
            std::tuple<float, float, float> p2 = calc_dist(plane.p2, cam_vec, cam_len_sqrd, y_vec, x_vec);
            float p2x = std::get<0>(p2), p2y = std::get<1>(p2), p2z = std::get<2>(p2);
            std::tuple<float, float, float> p3 = calc_dist(plane.p3, cam_vec, cam_len_sqrd, y_vec, x_vec);
            float p3x = std::get<0>(p3), p3y = std::get<1>(p3), p3z = std::get<2>(p3);
            std::tuple<float, float, float> p4 = calc_dist(plane.p4, cam_vec, cam_len_sqrd, y_vec, x_vec);
            float p4x = std::get<0>(p4), p4y = std::get<1>(p4), p4z = std::get<2>(p4);

            sf::ConvexShape quad;
            quad.setPointCount(4);

            quad.setPoint(0, sf::Vector2f(3*SCREEN_HEIGHT/5 + (p1x)/(p1z)*500, SCREEN_HEIGHT/2 - (p1y)/(p1z)*500));
            quad.setPoint(1, sf::Vector2f(3*SCREEN_HEIGHT/5 + (p2x)/(p2z)*500, SCREEN_HEIGHT/2 - (p2y)/(p2z)*500));
            quad.setPoint(2, sf::Vector2f(3*SCREEN_HEIGHT/5 + (p3x)/(p3z)*500, SCREEN_HEIGHT/2 - (p3y)/(p3z)*500));
            quad.setPoint(3, sf::Vector2f(3*SCREEN_HEIGHT/5 + (p4x)/(p4z)*500, SCREEN_HEIGHT/2 - (p4y)/(p4z)*500));

            sf::Color fill = plane.color;
            fill.a = 99;
            quad.setFillColor(fill);

            window.draw(quad);

            sf::Text label(std::to_string(i+1) + ". " + plane.text, font, 14);
            label.setFillColor(plane.color);
            label.setPosition(sf::Vector2f(25, 115+50*(vectors.size() + i)));
            window.draw(label);
        }

        for (BasisVector& bv : basis_vectors) {

            Vector3 vec = bv.vec;

            std::tuple<float, float, float> tup = calc_dist(vec, cam_vec, cam_len_sqrd, y_vec, x_vec);
            float x_dist = std::get<0>(tup), y_dist = std::get<1>(tup), z_dist = std::get<2>(tup);

            sf::Vector2f tip(3*SCREEN_HEIGHT/5 + (x_dist)/(z_dist)*500, SCREEN_HEIGHT/2 - (y_dist)/(z_dist)*500);

            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(3*SCREEN_HEIGHT/5, SCREEN_HEIGHT/2);
            line[1].position = tip;

            window.draw(line);

            sf::Text label(bv.text, font, 14);
            label.setPosition(tip);
            window.draw(label);
        }

        sf::Text vec_header("Vectors:", font, 20);
        vec_header.setPosition(sf::Vector2f(15, 15));
        window.draw(vec_header);

        for (int i = 0; i < vectors.size(); i++) {
            Vector vector = vectors[i];
            Vector3 vec = vector.vec;

            std::tuple<float, float, float> tup = calc_dist(vec, cam_vec, cam_len_sqrd, y_vec, x_vec);
            float x_dist = std::get<0>(tup), y_dist = std::get<1>(tup), z_dist = std::get<2>(tup);

            sf::Vector2f tip(3*SCREEN_HEIGHT/5 + (x_dist)/(z_dist)*500, SCREEN_HEIGHT/2 - (y_dist)/(z_dist)*500);
            
            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(3*SCREEN_HEIGHT/5, SCREEN_HEIGHT/2);
            line[1].position = tip;

            line[0].color = vector.color;
            line[1].color = vector.color;

            window.draw(line);

            sf::Text label(std::to_string(i+1) + ". " + vector.text, font, 14);
            label.setFillColor(vector.color);
            label.setPosition(tip);
            window.draw(label);
            label.setPosition(sf::Vector2f(25, 65+50*i));
            window.draw(label);
        }   

        sf::Text cmd("Command: [" + cur_command + "]\n(try 'help' for assistance)", font, 20);
        cmd.setPosition(sf::Vector2f(30, SCREEN_HEIGHT-60));
        window.draw(cmd);

        sf::Text error(e_msg, font, 20);
        error.setFillColor(sf::Color::Red);
        error.setPosition(sf::Vector2f(30, SCREEN_HEIGHT-120));
        window.draw(error);

        sf::Text help(help_msg, font, 20);
        help.setFillColor(sf::Color::Yellow);
        help.setPosition(sf::Vector2f(30, SCREEN_HEIGHT-400));
        window.draw(help);

        window.display();
        // stop drawing
    }

    return 0;
}