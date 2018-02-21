#ifndef planetoidgameobject_hpp
#define planetoidgameobject_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "utilities/Hex.hpp"
#include "utilities/constants.hpp"
#include "utilities/utilities.hpp"

class PlanetoidGameObject
{
public:
    // game state
    int rotation, num_sides, size, mass;
    bool has_atmosphere, has_ocean;
    int ocean_level;
    Hex position;
    // unordered list of hexes considered to be areabrake areas for spacecraft
    std::vector<Hex> atmosphere_collision;
    // unordered list of hexes considered to be collision areas for spacecraft
    std::vector<Hex> collision;
    // ordered list of surface hexes, clockwise from rightmost at rotation=0
    std::vector<Hex> surface;
    std::vector<int> elevations;
    std::vector<bool> ocean_present;

    PlanetoidGameObject(int size, Hex position);

    int find_landing_location(Hex pos, Hex prev_pos);
    Hex get_landed_ship_position(int landing_location);

    void update();
};

#endif