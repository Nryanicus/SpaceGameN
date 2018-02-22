#include "ShipRenderer.hpp"

ShipRenderer::ShipRenderer(ShipGameObject* ship)
: ship(ship),
  blink(false), elapsed_time(0)
{
    ship_array.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: MILITARY_SHIP)
        ship_array.append(sf::Vertex(v.to_sfml(), SHIP_COLOUR));

    dashed_ship_array.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: DASHED_MILITARY_SHIP)
        dashed_ship_array.append(sf::Vertex(v.to_sfml(), DASHED_SHIP_COLOUR));
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
        pos -= (p.normalise()*LANDED_SHIP_OFFSET).to_sfml();
    }
    else
    {
        rot = 60*ship->rotation;
        pos = axial_to_pixel(ship->position.q, ship->position.r).to_sfml();
    }

    sf::Transform trans;
    trans.translate(pos);
    trans.rotate(rot);
    target->draw(ship_array, sf::RenderStates(trans));

    // draw next position
    if (!blink && (ship->velocity != Hex() || ship->taking_off))
    {
        if (debug)
            for (Hex movement_hex: (ship->position+ship->velocity).all_hexes_between(ship->position))
                movement_hex.draw(target, true, sf::Color(50, 255, 50, 100));
        rot = 60*ship->rotation;
        pos = axial_to_pixel(ship->position.q+ship->velocity.q, ship->position.r+ship->velocity.r).to_sfml();
        trans = sf::Transform();
        trans.translate(pos);
        trans.rotate(rot);
        target->draw(dashed_ship_array, sf::RenderStates(trans));
    }
    elapsed_time += dt;
    if (elapsed_time > 0.5)
    {
        elapsed_time = 0;
        blink = ! blink;
    }
}