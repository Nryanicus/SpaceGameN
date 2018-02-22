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

const sf::Color TIRONIUM_COLOUR = sf::Color(200,50,50);
const sf::Color CARBONICUM_COLOUR = sf::Color(100,175,175);
const sf::Color HELIDROGEN_COLOUR = sf::Color(0,100,175);
const sf::Color FUNDAMENTIUM_COLOUR = sf::Color(25,25,25);

class PlanetoidRenderer
{
private:
    std::vector<Ocean> oceans;
    
    // gravity
    std::vector<sf::ConvexShape> gravity_shapes;
    double gravity_wave_radius;
    double elapsed_time;
    sf::ConvexShape gravity_wave;
    sf::Font font;

    sf::VertexArray atmosphere;

    // body
    sf::VertexArray outline;
    sf::VertexArray fill;

    // resources
    sf::Sprite surface_tironium;
    sf::Sprite subterranean_tironium;
    sf::Sprite surface_carbicon;
    sf::Sprite subterranean_carbicon;
    sf::Sprite surface_helidrogen;
    sf::Sprite subterranean_fundamentium;

    sf::Texture surface_tironium_texture;
    sf::Texture subterranean_tironium_texture;
    sf::Texture surface_carbicon_texture;
    sf::Texture subterranean_carbicon_texture;
    sf::Texture surface_helidrogen_texture;
    sf::Texture subterranean_fundamentium_texture;

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

    double get_landing_position_angle(int landing_location, Vector* v, bool subterranean=false);
};

#endif