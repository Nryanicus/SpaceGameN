#include "utilities.hpp"

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
Vector Vector::operator*(float m)
{
    return Vector(m*x, m*y);
}
Vector Vector::operator*=(float m)
{
    x *= m;
    y *= m;
    return *this;
}    
Vector Vector::operator/(float m)
{
    return Vector(x/m, y/m);
}
Vector Vector::operator/=(float m)
{
    x /= m;
    y /= m;
    return *this;
}
float Vector::magnitude()
{
    return sqrt(x*x + y*y);
}
float Vector::magnitude_squared()
{
    return x*x + y*y;
}
Vector Vector::normalise()
{
    float m = magnitude();
    return (*this)/m;
}
float Vector::dot(Vector other)
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

std::default_random_engine RandomGenerator(time(NULL));

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
    os << "V: " << v.x << ", " << v.y;
    return os;
}

//////////////////////
/*      Hex Ops     */
//////////////////////

std::pair<int, int> round_hex(float q, float r)
{
    float x = q;
    float z = r;
    float y = -x-z;

    int rx = (int) round(x);
    int ry = (int) round(y);
    int rz = (int) round(z);

    int x_diff = abs(rx - x);
    int y_diff = abs(ry - y);
    int z_diff = abs(rz - z);

    if ((x_diff > y_diff) && (x_diff > z_diff))
        rx = -ry-rz;
    else if (y_diff > z_diff)
        ry = -rx-rz;
    else
        rz - -rx-ry;

    return std::make_pair(rx, rz);
}

std::pair<int, int> pixel_to_axial(float x, float y)
{
    float q = x*SQRT3/3.0 - y/3.0;
    float r = y*2/3.0;
    return round_hex(q, r);
}

Vector axial_to_pixel(float q, float r)
{
    float x = SQRT3*(q+r/2.0);
    float y = 3/2.0*r;
    return Vector(x, y);
}

//////////////////////
/*     Misc Ops     */
//////////////////////

/* return random int in range [a, b], assumes srand already called*/
int random_int(int a, int b)
{
    if (a==b) return a;
    assert (a<b);
    std::uniform_int_distribution<int> distr(a, b);
    int c = distr(RandomGenerator);
    assert (c>=a && c<=b);
    return c;
}