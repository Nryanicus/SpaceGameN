#ifndef planetoid_hpp
#define planetoid_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "utilities/Hex.hpp"
#include "utilities/constants.hpp"
#include "utilities/utilities.hpp"

class Planetoid
{
private:
    sf::ConvexShape shape;
    double radius;
public:
    int size;
    int mass;
    Hex position;
    // unordered list of hexes considered to be collision areas for spacecraft
    std::vector<Hex> collision;
    // ordered list of surface hexes, clockwise from rightmost at rotation=0
    std::vector<Hex> surface;
    Planetoid(int size, Hex position);
    void draw(sf::RenderTarget* target, bool debug=false);

    int find_landing_location(Hex pos, Hex prev_pos, Hex* h);
    double get_landing_position_angle(int landing_location, Vector* v);
};

#endif