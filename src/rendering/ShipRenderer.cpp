#include "ShipRenderer.hpp"

ShipRenderer::ShipRenderer(ShipGameObject* ship)
: ship(ship),
  rotation(0),
  blink(false), elapsed_time_blink(0),
  elapsed_time_plume(0),
  pathfinding_state(PathfindUIState::Awaiting)
{
    ship_array.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: MILITARY_SHIP)
        ship_array.append(sf::Vertex(v.to_sfml(), SHIP_COLOUR));

    dashed_ship_array.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: DASHED_MILITARY_SHIP)
        dashed_ship_array.append(sf::Vertex(v.to_sfml(), DASHED_SHIP_COLOUR));

    plume_array.setPrimitiveType(sf::PrimitiveType::Lines);
    plume_array.resize(8);

    dashed_plume_array.setPrimitiveType(sf::PrimitiveType::Lines);
    dashed_plume_array.resize(8);
}

void ShipRenderer::draw(sf::RenderTarget* target, double dt, bool debug)
{
    // if ship has received input ensure the preview is visible
    if (ship->reset_position_preview)
    {
        blink = false;
        elapsed_time_blink = 0;
        ship->reset_position_preview = false;
    }

    sf::Vector2f pos;
    double rot;
    bool draw_burn = false;
    if (ship->landed)
    {
        Vector p;
        rot = ship->landed_planetoid->get_landing_position_angle(ship->landed_location, &p);
        pos = p.to_sfml();
        // FIXME
        pos -= (p.normalise()*LANDED_SHIP_OFFSET).to_sfml();
    }
    else
    {
        if (ship->accelerating != -1)
        {
            rotation = ship->accelerating;
            draw_burn = true;
        }
        rot = 60*rotation;
        pos = axial_to_pixel(ship->position.q, ship->position.r).to_sfml();
    }

    sf::Transform trans;
    trans.translate(pos);
    trans.rotate(rot);
    target->draw(ship_array, sf::RenderStates(trans));
    if (draw_burn)
        target->draw(plume_array, sf::RenderStates(trans));

    // draw next position
    if (!blink && (ship->velocity != Hex() || ship->taking_off))
    {
        if (debug)
            for (Hex movement_hex: (ship->position+ship->velocity).all_hexes_between(ship->position))
                movement_hex.draw(target, true, sf::Color(50, 255, 50, 100));
        
        // show planned burn
        rot = 60*rotation;
        draw_burn = false;
        if (ship->planned_accelerations.size() != 0)
            if (ship->planned_accelerations[0] != Hex())
            {
                rot = 60*dirc_to_index(ship->planned_accelerations[0]);
                draw_burn = true;
            }

        pos = axial_to_pixel(ship->position.q+ship->velocity.q, ship->position.r+ship->velocity.r).to_sfml();
        trans = sf::Transform();
        trans.translate(pos);
        trans.rotate(rot);
        target->draw(dashed_ship_array, sf::RenderStates(trans));
        if (draw_burn)
            target->draw(dashed_plume_array, sf::RenderStates(trans));
    }

    elapsed_time_blink += dt;
    if (elapsed_time_blink > 0.5)
    {
        elapsed_time_blink = 0;
        blink = ! blink;
    }
    update_plumes(dt);
}

void ShipRenderer::update_plumes(double dt)
{
    elapsed_time_plume += dt;
    if (elapsed_time_plume > PLUME_ANIMATE_TIME)
        elapsed_time_plume = 0;
    double r = elapsed_time_plume/PLUME_ANIMATE_TIME;
    Vector v1 = MILITARY_SHIP_PLUME[0][0];
    Vector v2 = MILITARY_SHIP_PLUME[1][0];
    plume_array[0] = sf::Vertex((v1*r + v2*(1-r)).to_sfml(), PLUME_COLOUR);
    v1 = MILITARY_SHIP_PLUME[0][1];
    v2 = MILITARY_SHIP_PLUME[1][1];
    plume_array[1] = sf::Vertex((v1*r + v2*(1-r)).to_sfml(), PLUME_COLOUR);
    
    for (int i=0; i<8; i+=2)
    {
        v1 = DASHED_MILITARY_SHIP_PLUME[0][i];
        v2 = DASHED_MILITARY_SHIP_PLUME[1][i];
        dashed_plume_array[i] = sf::Vertex((v1*r + v2*(1-r)).to_sfml(), PLUME_COLOUR);
        v1 = DASHED_MILITARY_SHIP_PLUME[0][i+1];
        v2 = DASHED_MILITARY_SHIP_PLUME[1][i+1];
        dashed_plume_array[i+1] = sf::Vertex((v1*r + v2*(1-r)).to_sfml(), PLUME_COLOUR);
    }
}

void ShipRenderer::take_path_input(Hex h)
{
    if (pathfinding_state == PathfindUIState::Awaiting)
    {
        goal_position = h;
        pathfinding_state = PathfindUIState::NeedVelocity;
    }
    else if (pathfinding_state == PathfindUIState::NeedVelocity)
    {
        ship->pathfind_to(goal_position, h-goal_position);
        pathfinding_state = PathfindUIState::Awaiting;
    }
}