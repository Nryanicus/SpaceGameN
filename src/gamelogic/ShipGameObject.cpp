#include "ShipGameObject.hpp"

ShipGameObject::ShipGameObject(Hex pos, std::vector<Planetoid*>* planets)
: position(pos), velocity(Hex(0,0)), rotation(0),
  planets(planets), 
  landed(false), taking_off(false), landed_planetoid(NULL), landed_location(-1),
  reset_position_preview(false)
{}

void ShipGameObject::update()
{
    position += velocity;
    if (landed)
    {
        // skip landing, gravity and crash checks if we're landed and aren't taking off
        if (velocity == Hex() && !taking_off) 
        {
            position = landed_planetoid->get_landed_ship_position(landed_location);
            return;
        }
        else
        {
            landed = false;
            landed_location = -1;
            landed_planetoid = NULL;
            taking_off = false;
        }
    }
    // check for landing
    else
        for (Planetoid* p : *planets)
        {
            if (position.distance(p->position) <= p->size && velocity.magnitude()<=1)
            {
                landed_location = p->find_landing_location(position, position-velocity);
                if (landed_location != -1)
                {
                    landed = true;
                    landed_planetoid = p;
                    velocity = Hex();
                    return;
                }
            }
        }

    // check for crash, scanning all hexes that were moved through
    // check for areobrake
    for (Planetoid* p : *planets)
        for (Hex movement_hex: (position-velocity).all_hexes_between(position))
        {
            if (movement_hex.distance(p->position) <= p->size)
                for (Hex collision_hex: p->collision)
                    if ((collision_hex+p->position) == movement_hex)
                        std::cout << "crash! " << p->size << std::endl;
            if (movement_hex.distance(p->position) <= p->size+2 && velocity != Hex())
                for (Hex atmo_hex: p->atmosphere_collision)
                    if ((atmo_hex+p->position) == movement_hex)
                    {
                        std::vector<Hex> new_vel = Hex().all_hexes_between(velocity);
                        velocity = new_vel[new_vel.size()-2];
                        if (velocity == Hex())
                            break;
                    }
        }

    // check for gravity
    for (Planetoid* p : *planets)
    {
        int distance = position.distance(p->position);
        // assert (distance != 0 && "ShipGameObject inside planet core");
        if (distance == 0) std::cout << "ShipGameObject inside planet core" << std::endl;
        int gravity = p->mass - distance;
        // if too far away, do nothing
        if (gravity <= 0) continue;
        // otherwise accelerate towards planet
        Hex dirc = position.all_hexes_between(p->position)[1] - position;
        velocity += gravity*dirc;
    }

}

void ShipGameObject::rotate(int dirc)
{
    reset_position_preview = true;
    if (dirc == 1)
        rotation = (1+rotation)%6;
    else if (dirc == -1)
        rotation = rotation==0 ? 5 : rotation-1;
    else
        assert(false && "invalid dirc argument value to ShipGameObject::rotate");
}

void ShipGameObject::accelerate(int mag)
{
    reset_position_preview = true;
    if (landed && !taking_off) 
        taking_off = true;
    else
        velocity += rotate_hex(Hex(1, 0), rotation)*mag;
}