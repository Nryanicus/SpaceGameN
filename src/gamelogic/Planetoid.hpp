#ifndef planetoid_hpp
#define planetoid_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "utilities/Hex.hpp"
#include "utilities/constants.hpp"
#include "utilities/utilities.hpp"

const sf::Color GRAVITY_COLOUR = sf::Color(10,20,10);
const sf::Color GRAVITY_WAVE_COLOUR = sf::Color(200,50,200);
const sf::Color ATMOSPHERE_COLOUR = sf::Color(25,50,100,128);

const double ELEVATION_QUANTUM = 0.25;

class Planetoid
{
private:
    // game state
    int rotation;

    // rendering
    std::vector<sf::ConvexShape> gravity_shapes;
    double gravity_wave_radius;
    sf::ConvexShape gravity_wave;

    sf::VertexArray atmosphere;

    sf::VertexArray outline;
    sf::VertexArray fill;
    sf::Font font;

    std::vector<double> radii;

    // cache
    std::vector<double> landing_angles;
    std::vector<Vector> landing_locations;

public:
    // game state
    int size;
    int mass;
    Hex position;
    // unordered list of hexes considered to be areabrake areas for spacecraft
    std::vector<Hex> atmosphere_collision;
    // unordered list of hexes considered to be collision areas for spacecraft
    std::vector<Hex> collision;
    // ordered list of surface hexes, clockwise from rightmost at rotation=0
    std::vector<Hex> surface;
    Planetoid(int size, Hex position);
    void draw(sf::RenderTarget* target, double dt, bool gravity, bool debug=false);

    int find_landing_location(Hex pos, Hex prev_pos);
    double get_landing_position_angle(int landing_location, Vector* v);
    Hex get_landed_ship_position(int landing_location);

    void update();
};

#endif