#include "utilities.hpp"

//////////////////////
/*      Hex Ops     */
//////////////////////

std::pair<int, int> round_hex(double q, double r)
{
    double x = q;
    double z = r;
    double y = -x-z;

    return round_hex(x, y, z);
}

std::pair<int, int> round_hex(double x, double y, double z)
{
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
        rz = -rx-ry;

    return std::make_pair(rx, rz);
}

std::pair<int, int> pixel_to_axial(double x, double y)
{
    double q = (x*SQRT3/3.0 - y/3.0) / HEX_SIZE;
    double r = y*2/3.0               / HEX_SIZE;
    return round_hex(q, r);
}

Vector axial_to_pixel(double q, double r)
{
    double x = SQRT3*(q+r/2.0) * HEX_SIZE;
    double y = 3/2.0*r         * HEX_SIZE;
    return Vector(x, y);
}

//////////////////////
/*     Misc Ops     */
//////////////////////

// seed pseudorandom generator
std::default_random_engine RandomGenerator(time(NULL));

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

double lerp(double a, double b, double t)
{
    return a + (b - a)*t;
}