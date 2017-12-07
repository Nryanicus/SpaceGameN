#ifndef vector_hpp
#define vector_hpp

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "constants.hpp"

class Vector
{
public:
    double x,y;
    Vector() : x(0), y(0) {}
    Vector(float x, float y) : x(x), y(y) {}
    Vector(double x, double y) : x(x), y(y) {}
    Vector(int x, int y) : x((double)x), y((double)y) {}
    Vector(sf::Vector2f v) : x(v.x), y(v.y) {}
    Vector(sf::Vector2i v) : x((double)v.x), y((double)v.y) {}
    Vector operator-(Vector o);
    Vector operator-=(Vector o);
    Vector operator+(Vector o);
    Vector operator+=(Vector o);
    Vector operator*(double m);
    Vector operator*=(double m);
    Vector operator/(double m);
    Vector operator/=(double m);
    double magnitude();
    double magnitude_squared();
    Vector normalise();
    double dot(Vector other);
    Vector projection(Vector other);
    double angle_between(Vector other);
    Vector rotate(double rot);
    sf::Vector2f to_sfml() const;
};

std::ostream& operator<<(std::ostream& os, const Vector& v);

#endif