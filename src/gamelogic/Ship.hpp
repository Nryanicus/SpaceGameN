#ifndef ship_hpp
#define ship_hpp

#include <SFML/Graphics.hpp>
#include "utilities/Hex.hpp"
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
    bool landed, taking_off;
    Planetoid* landed_planetoid;
    int landed_location;

    // rendering
    sf::Texture ship_texture;
    sf::Texture ship_dashed_texture;
    sf::Sprite sprite;
    bool blink;
    double elapsed_time;

    Ship(Hex pos, std::vector<Planetoid*>* planets);
    void update();
    void draw(sf::RenderTarget* target, double dt, bool debug=false);

    void rotate(int dirc);
    void accelerate(int mag);
};

#endif