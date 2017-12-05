#ifndef utilities_hpp
#define utilities_hpp

#include <SFML/Graphics.hpp>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "constants.hpp"

class Vector
{
public:
    float x,y;
    Vector() : x(0), y(0) {}
    Vector(float x, float y) : x(x), y(y) {}
    Vector(double x, double y) : x(x), y(y) {}
    Vector(int x, int y) : x((float)x), y((float)y) {}
    Vector(sf::Vector2f v) : x(v.x), y(v.y) {}
    Vector(sf::Vector2i v) : x((float)v.x), y((float)v.y) {}
    Vector operator-(Vector o);
    Vector operator-=(Vector o);
    Vector operator+(Vector o);
    Vector operator+=(Vector o);
    Vector operator*(float m);
    Vector operator*=(float m);
    Vector operator/(float m);
    Vector operator/=(float m);
    float magnitude();
    float magnitude_squared();
    Vector normalise();
    float dot(Vector other);
    Vector projection(Vector other);
    double angle_between(Vector other);
};

std::ostream& operator<<(std::ostream& os, const Vector& v);

//////////////////////
/*      Hex Ops     */
//////////////////////

std::pair<int, int> round_hex(float q, float r);

std::pair<int, int> pixel_to_axial(float x, float y);

Vector axial_to_pixel(float q, float r);

//////////////////////
/*     Misc Ops     */
//////////////////////

int random_int(int a, int b);

#endif