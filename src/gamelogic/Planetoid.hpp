#ifndef planetoid_hpp
#define planetoid_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include "utilities/Hex.hpp"
#include "utilities/constants.hpp"
#include "utilities/utilities.hpp"

class Planetoid
{
private:
    sf::ConvexShape shape;
public:
    int size;
    int mass;
    Hex position;
    Planetoid(int size, Hex position);
    void draw(sf::RenderTarget* target);

    int find_landing_location(Hex pos, Hex prev_pos);
};

#endif
