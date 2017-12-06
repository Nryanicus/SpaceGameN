#ifndef ship_hpp
#define ship_hpp

#include <SFML/Graphics.hpp>
#include "Hex.hpp"
#include "Planetoid.hpp"

class Ship
{
public:
    // flight state
    Hex position;
    Hex velocity;
    int rotation;

    std::vector<Planetoid*>* planets;

    // landed state
    bool landed;
    Planetoid* landed_planetoid;
    int landed_location;

    // rendering
    sf::Texture ship_texture;
    sf::Texture ship_dashed_texture;
    sf::Sprite sprite;

    Ship(Hex pos, std::vector<Planetoid*>* planets);
    void update();
    void draw(sf::RenderTarget* target);

    void rotate(int dirc);
    void accelerate(int mag);
};

#endif