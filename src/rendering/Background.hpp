#ifndef background_hpp
#define background_hpp

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "utilities/Hex.hpp"
#include "utilities/utilities.hpp"

const sf::Color BG_HEX_COLOUR = sf::Color(255, 255, 0, 50);
const sf::Color TEXT_FILL_COLOUR = sf::Color(255, 255, 255, 200);

class Background
{
private:
    sf::Texture hex_texture;
    sf::Sprite hex_sprite;
    sf::Font font;
public:
    Background();
    void draw(sf::RenderTarget* target, double zoom, bool hex_nums=false);
};

#endif