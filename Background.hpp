#ifndef background_hpp
#define background_hpp

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <unordered_map>
#include <iostream>
#include "Hex.hpp"
#include "utilities.hpp"

const sf::Color BG_HEX_COLOUR = sf::Color(255, 255, 0, 50);

class Background
{
private:
    sf::Texture hex_texture;
    sf::Sprite hex_sprite;
public:
    Background();
    void draw(sf::RenderTarget* target, double zoom);
};

#endif