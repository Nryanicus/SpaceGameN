#ifndef background_hpp
#define background_hpp

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <unordered_map>
#include <iostream>
#include "Hex.hpp"
#include "utilities.hpp"
#include "constants.hpp"

class Background
{
private:
    sf::Texture tile_texture;
    sf::Texture single_hex_texture;
    sf::Sprite tile;
    sf::VertexArray hexes;
    std::unordered_map<Hex, sf::Vertex*> hex_to_vertex;
    // size of array hexes is drawing
public:
    Background();
    void draw(sf::RenderTarget* target, sf::Font* font, float zoom);
    void paint_hex(Hex to_paint, sf::Color col);
    void paint_hexes(std::vector<Hex> to_paint, sf::Color col);
    void clear_hexes(std::vector<Hex> to_paint);
    void clear_all_hexes();
};

#endif