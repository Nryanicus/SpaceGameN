#include "ShipGameObject.hpp"

ShipGameObject::ShipGameObject(Hex pos, std::vector<Planetoid*>* planets)
: position(pos), velocity(Hex(0,0)),
  planets(planets), 
  landed(false), taking_off(false), landed_planetoid(NULL), landed_location(-1),
  reset_position_preview(false), accelerating(-1)
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

    // check for crash, scanning all hexes that were moved through
    // check for areobrake
    for (Planetoid* p : *planets)
    {
        Hex crash;
        if (p->collision_in_path(position-velocity, position, &crash))
            std::cout << "crash at " << crash << std::endl;
        // // aerobrake
        // for (Hex movement_hex: (position-velocity).all_hexes_between(position))
        //     if (movement_hex.distance(p->position) <= p->size+2 && velocity != Hex())
        //         for (Hex atmo_hex: p->atmosphere_collision)
        //             if ((atmo_hex+p->position) == movement_hex)
        //             {
        //                 std::vector<Hex> new_vel = Hex().all_hexes_between(velocity);
        //                 velocity = new_vel[new_vel.size()-2];
        //                 std::cout << "areobrake at " << movement_hex << std::endl;
        //                 if (velocity == Hex())
        //                     break;
        //             }
    }

    // check for gravity
    for (Planetoid* p : *planets)
        velocity += p->get_gravity_at_point(position);

    if (planned_accelerations.size() != 0)
    {
        velocity += planned_accelerations.front();
        if (planned_accelerations.front() != Hex())
            accelerating = dirc_to_index(planned_accelerations.front());
        else
            accelerating = -1;
        planned_accelerations.pop_front();
        // std::cout << "state: " << position << " " << velocity << std::endl;
    }
    else
        accelerating = -1;
}

void ShipGameObject::pathfind_to(Hex goal_pos, Hex goal_vel)
{
    planned_accelerations = pathfind(position, velocity, goal_pos, goal_vel, 1, planets);
    if (planned_accelerations.size() == 0)
        std::cout << "failed to find path" << std::endl;
    else
    {
        velocity += planned_accelerations.front();
        if (planned_accelerations.front() != Hex())
            accelerating = dirc_to_index(planned_accelerations.front());
        else
            accelerating = -1;
        planned_accelerations.pop_front();
        // std::cout << "state: " << position << " " << velocity << std::endl;
    }
}