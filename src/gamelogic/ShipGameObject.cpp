#include "ShipGameObject.hpp"

ShipGameObject::ShipGameObject(Hex pos, std::vector<Planetoid*>* planets)
: acceleration(Hex(0,0)), position(pos), velocity(Hex(0,0)),
  planets(planets), 
  landed(false), taking_off(false), landed_planetoid(NULL), landed_location(-1),
  reset_position_preview(false), 
  pathfind_from_orbit(false), pathfinding_state(PathfindingState::None)
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
    }

    // check for gravity
    for (Planetoid* p : *planets)
        velocity += p->get_gravity_at_point(position);

    // do pathfinding
    check_pathfinding();

    // do planned burns
    if (planned_accelerations.size() != 0)
    {
        acceleration = planned_accelerations.front();
        velocity += acceleration;
        if (acceleration != Hex())
        {
            std::vector<Hex> acceleration_line = acceleration.all_hexes_between(Hex());
            int s = acceleration_line.size();
            assert(s >= 2);
            rotation = dirc_to_index(acceleration_line[s-2]);
        }
        planned_accelerations.pop_front();
        // std::cout << "state: " << position << " " << velocity << std::endl;
    }
    else
        acceleration = Hex();
}

void ShipGameObject::check_pathfinding()
{
    // check if we're pathfinding and if so if we've got a result yet
    // otherwise return
    if (!pathfinder_return.valid() || pathfinder_return.wait_for(NO_TIME) != std::future_status::ready)
        return;

    // check if we've found a solution but are in an incorrect orbital position
    if (pathfind_from_orbit && position != orbital_start_pos)
    {
        pathfinding_state = PathfindingState::Waiting;
        return;
    }

    // we're ready to execute our found solution, or report we didn't find one
    planned_accelerations = pathfinder_return.get();

    if (planned_accelerations.size() == 0)
        pathfinding_state = PathfindingState::PathNotFound;
    else
        pathfinding_state = PathfindingState::PathFound;

    pathfind_from_orbit = false;
}

bool ShipGameObject::is_in_orbit()
{
    for (Planetoid* p : *planets)
    {
        int goldilocks = p->get_gravity_at_point(position).distance(Hex());
        // can't be in orbit if we're not close enough
        if (p->position.distance(position) != goldilocks) continue;
        // can't be in orbit if we're not in an appropriate hex
        bool in_cardinal_hex = false;
        int i=0;
        for (Hex h : CARDINAL_DIRECTIONS)
        {
            if (position - p->position == h*goldilocks)
            {
                in_cardinal_hex = true;
                break;
            }
            i++;
        }
        if (!in_cardinal_hex) continue;
        // can't be in orbit if our velocity is not correct
        int j = (i != 0) ? i-1 : 5;
        i = (i+1)%6;
        Hex local_next_pos = velocity+position-p->position;
        Hex clockwise_next_pos = CARDINAL_DIRECTIONS[i];
        clockwise_next_pos *= goldilocks;
        Hex counterclockwise_next_pos = CARDINAL_DIRECTIONS[j];
        counterclockwise_next_pos *= goldilocks;
        if (local_next_pos == clockwise_next_pos || local_next_pos == counterclockwise_next_pos)
            return true;
    }
    return false;
}

void ShipGameObject::pathfind_to(Hex goal_pos, Hex goal_vel)
{
    if (pathfinding_state != PathfindingState::None) return;
    if (is_in_orbit())
    {
        pathfind_from_orbit = true;
        orbital_start_pos = position;
    }
    else if (velocity != Hex()) 
        return;
    std::vector<PlanetoidGameObject*>* planets_local = new std::vector<PlanetoidGameObject*>();
    for (Planetoid* p: *planets)
        planets_local->push_back(new PlanetoidGameObject(p));
    pathfinder_return = std::async(std::launch::async, pathfind, position, velocity, goal_pos, goal_vel, 1, planets_local);
    pathfinding_state = PathfindingState::Computing;
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
    {
        acceleration = rotate_hex(Hex(1, 0), rotation)*mag;
        velocity += acceleration;
    }
}

void ShipGameObject::cancel_pathfinding()
{
    pathfinding_state = PathfindingState::None;
    planned_accelerations.clear();
}
