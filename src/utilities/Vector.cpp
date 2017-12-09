#include "Vector.hpp"

//////////////////////
/* Vector Class def */
//////////////////////

Vector Vector::operator-(Vector o)
{
    return Vector(x-o.x, y-o.y);
}
Vector Vector::operator-=(Vector o)
{
    x -= o.x;
    y -= o.y;
    return *this;
}
Vector Vector::operator+(Vector o)
{
    return Vector(x+o.x, y+o.y);
}
Vector Vector::operator+=(Vector o)
{
    x += o.x;
    y += o.y;
    return *this;
}
Vector Vector::operator*(double m)
{
    return Vector(m*x, m*y);
}
Vector Vector::operator*=(double m)
{
    x *= m;
    y *= m;
    return *this;
}    
Vector Vector::operator/(double m)
{
    return Vector(x/m, y/m);
}
Vector Vector::operator/=(double m)
{
    x /= m;
    y /= m;
    return *this;
}
double Vector::magnitude()
{
    return sqrt(x*x + y*y);
}
double Vector::magnitude_squared()
{
    return x*x + y*y;
}
Vector Vector::normalise()
{
    double m = magnitude();
    return (*this)/m;
}
double Vector::dot(Vector other)
{
    return x*other.x + y*other.y;
}
Vector Vector::projection(Vector other)
{
    return other * dot(other)/other.dot(other);
}
double Vector::angle_between(Vector other)
{
    Vector v1 = normalise();
    Vector v2 = other.normalise();
    return acos(v1.dot(v2)) * RADIANS_TO_DEGREES;
}
Vector Vector::rotate(double rot)
{
    double nx = x*cos(rot) - y*sin(rot);
    double ny = x*sin(rot) + y*cos(rot);
    return Vector(nx, ny);
}
sf::Vector2f Vector::to_sfml() const
{
    return sf::Vector2f(x, y);
}

Vector operator*(double m, Vector v)
{
    return v*m;
}

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
    os << "V: " << v.x << ", " << v.y;
    return os;
}