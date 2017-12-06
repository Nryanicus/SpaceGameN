#ifndef planetoid_hpp
#define planetoid_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Hex.hpp"
#include "constants.hpp"
#include "utilities.hpp"

class Planetoid
{
private:
    int size;
    sf::ConvexShape shape;
public:
    int mass;
    Hex position;
    Planetoid(int size, Hex position);
    void draw(sf::RenderTarget* target);
};

#endif
