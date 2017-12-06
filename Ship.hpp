#ifndef ship_hpp
#define ship_hpp

#include <SFML/Graphics.hpp>
#include "Hex.hpp"
#include "Planetoid.hpp"

class Ship
{
public:
    Hex position;
    Hex velocity;
    int rotation;

    sf::Sprite sprite;

    std::vector<Planetoid*>* planets;

    Ship(Hex pos, sf::Texture* tex, std::vector<Planetoid*>* planets);
    void update();
    void draw(sf::RenderTarget* target);

    void rotate(int dirc);
    void accelerate(int mag);
};

#endif