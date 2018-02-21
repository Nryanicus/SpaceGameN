#ifndef shiprenderer_hpp
#define shiprenderer_hpp

#include <SFML/Graphics.hpp>
#include "gamelogic/ShipGameObject.hpp"
#include "utilities/Hex.hpp"

class ShipRenderer
{
private:
    // state
    ShipGameObject* ship;

    // rendering
    sf::Texture ship_texture;
    sf::Texture ship_dashed_texture;
    sf::Sprite sprite;
    bool blink;
    double elapsed_time;
public:
    ShipRenderer(ShipGameObject* ship);
    void draw(sf::RenderTarget* target, double dt, bool debug=false);
};

#endif