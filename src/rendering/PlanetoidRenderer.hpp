#ifndef planetoidrenderer_hpp
#define planetoidrenderer_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "gamelogic/PlanetoidGameObject.hpp"
#include "rendering/Ocean.hpp"
#include "utilities/Hex.hpp"
#include "utilities/constants.hpp"
#include "utilities/utilities.hpp"

const sf::Color GRAVITY_COLOUR = sf::Color(10,20,10);
const sf::Color GRAVITY_WAVE_COLOUR = sf::Color(200,50,200);
const sf::Color ATMOSPHERE_COLOUR = sf::Color(25,50,100,128);

class PlanetoidRenderer
{
private:
    // rendering
    std::vector<Ocean> oceans;
    std::vector<sf::ConvexShape> gravity_shapes;
    double gravity_wave_radius;
    double elapsed_time;
    sf::ConvexShape gravity_wave;

    sf::VertexArray atmosphere;

    sf::VertexArray outline;
    sf::VertexArray fill;
    sf::Font font;

    // cache
    PlanetoidGameObject* planetoid;
    std::vector<double> landing_angles;
    std::vector<Vector> landing_locations;

    // rendering cache
    sf::Text gravity_text;
    sf::CircleShape gravity_circle;
    std::vector<Vector> gravity_positions;

public:
    PlanetoidRenderer(PlanetoidGameObject* p);
    void draw(sf::RenderTarget* target, double dt, bool gravity, bool debug=false);

    double get_landing_position_angle(int landing_location, Vector* v);
};

#endif