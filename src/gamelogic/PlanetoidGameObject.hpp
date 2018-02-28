#ifndef planetoidgameobject_hpp
#define planetoidgameobject_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "utilities/Hex.hpp"
#include "utilities/constants.hpp"
#include "utilities/utilities.hpp"

const int TIRONIUM = 2;
const int CARBICON = 4;
const int HELIDROGEN = 8;
const int FUNDAMENTIUM = 16;

class PlanetoidGameObject
{
public:
    // id
    std::string name;
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
    std::vector<int> surface_resources;
    std::vector<int> subterranean_resources;

    PlanetoidGameObject(int size, Hex position, std::string name);

    int find_landing_location(Hex pos, Hex prev_pos);
    Hex get_landed_ship_position(int landing_location);

    Hex get_gravity_at_point(Hex point);
    bool collision_in_path(Hex start, Hex end, Hex* location=NULL);
    bool collision_at_point(Hex point);


    void update();
};

#endif