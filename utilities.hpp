#ifndef utilities_hpp
#define utilities_hpp

#include <SFML/Graphics.hpp>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "Vector.hpp"
#include "constants.hpp"

//////////////////////
/*      Hex Ops     */
//////////////////////

std::pair<int, int> round_hex(double q, double r);
std::pair<int, int> round_hex(double x, double y, double z);

std::pair<int, int> pixel_to_axial(double x, double y);

Vector axial_to_pixel(double q, double r);

//////////////////////
/*     Misc Ops     */
//////////////////////

int random_int(int a, int b);

double lerp(double a, double b, double t);

#endif