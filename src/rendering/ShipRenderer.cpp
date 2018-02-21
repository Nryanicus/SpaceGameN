#include "ShipRenderer.hpp"

ShipRenderer::ShipRenderer(ShipGameObject* ship)
: ship(ship),
  blink(false), elapsed_time(0)
{
    ship_texture.loadFromFile("../res/sprites/ship.png");
    ship_texture.setSmooth(true);
    ship_dashed_texture.loadFromFile("../res/sprites/ship_dashed.png");
    ship_dashed_texture.setSmooth(true);
    sprite.setTexture(ship_texture);
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
    sprite.scale(0.5,0.5);
}

void ShipRenderer::draw(sf::RenderTarget* target, double dt, bool debug)
{
    // if ship has received input ensure the preview is visible
    if (ship->reset_position_preview)
    {
        blink = false;
        elapsed_time = 0;
        ship->reset_position_preview = false;
    }

    sf::Vector2f pos;
    double rot;
    if (ship->landed)
    {
        Vector p;
        rot = ship->landed_planetoid->get_landing_position_angle(ship->landed_location, &p);
        pos = p.to_sfml();
        sprite.setOrigin(0, sprite.getLocalBounds().height/2);
    }
    else
    {
        sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
        rot = 60*ship->rotation;
        pos = axial_to_pixel(ship->position.q, ship->position.r).to_sfml();
    }

    sprite.setColor(sf::Color(50, 255, 50, 200));
    sprite.setTexture(ship_texture);
    sprite.setRotation(rot);
    sprite.setPosition(pos);
    target->draw(sprite);

    // draw next position
    if (!blink && (ship->velocity != Hex() || ship->taking_off))
    {
        if (debug)
            for (Hex movement_hex: (ship->position+ship->velocity).all_hexes_between(ship->position))
                movement_hex.draw(target, true, sf::Color(50, 255, 50, 100));
        sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
        sprite.setColor(sf::Color(50, 255, 50, 100));
        sprite.setRotation(60*ship->rotation);
        sprite.setTexture(ship_dashed_texture);
        sprite.setPosition(axial_to_pixel(ship->position.q+ship->velocity.q, ship->position.r+ship->velocity.r).to_sfml());
        target->draw(sprite);
    }
    elapsed_time += dt;
    if (elapsed_time > 0.5)
    {
        elapsed_time = 0;
        blink = ! blink;
    }
}