#ifndef background_hpp
#define background_hpp

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <unordered_map>
#include <iostream>
#include "Hex.hpp"
#include "utilities.hpp"

class Background
{
public:
    Background();
    void draw(sf::RenderTarget* target, sf::Font* font, double zoom);
};

#endif