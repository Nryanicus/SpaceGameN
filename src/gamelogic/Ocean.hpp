#ifndef ocean_hpp
#define ocean_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include "utilities/constants.hpp"
#include "utilities/utilities.hpp"

const double ELEVATION_QUANTUM = 0.25;
const double OCEAN_SAMPLE_FREQ = 16;
const sf::Color OCEAN_COLOUR = sf::Color(0,0,255);

class Ocean
{
private:
    Vector start, end;
    sf::VertexArray vertex_array;
    double base_offset;
    void update(double elapsed_time);
public:
    // start, end local to Planetoid
    Ocean(Vector start, Vector end, double base_offset);
    void draw(sf::RenderTarget* target, sf::Transform trans, double elapsed_time);
};

#endif