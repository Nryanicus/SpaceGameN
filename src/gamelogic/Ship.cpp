#include "Ship.hpp"

Ship::Ship(Hex pos, std::vector<Planetoid*>* planets)
: position(pos), velocity(Hex(0,0)), rotation(0), planets(planets), landed(false),
  taking_off(false), landed_planetoid(NULL), landed_location(-1), blink(false), 
  elapsed_time(0)
{
    ship_texture.loadFromFile("../res/sprites/ship.png");
    ship_texture.setSmooth(true);
    ship_dashed_texture.loadFromFile("../res/sprites/ship_dashed.png");
    ship_dashed_texture.setSmooth(true);
    sprite.setTexture(ship_texture);
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
    sprite.scale(0.5,0.5);
}

void Ship::update()
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
            std::cout << "Takeoff!" << std::endl;
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
                    std::cout << "Landed!" << std::endl;
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
                        // std::cout << "areobrake! " << p->size << std::endl;
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
        assert (distance != 0 && "ship inside planet core");
        int gravity = p->mass - distance;
        // if too far away, do nothing
        if (gravity <= 0) continue;
        // otherwise accelerate towards planet
        Hex dirc = position.all_hexes_between(p->position)[1] - position;
        velocity += gravity*dirc;
    }

}

void Ship::draw(sf::RenderTarget* target, double dt, bool debug)
{
    sf::Vector2f pos;
    double rot;
    if (landed)
    {
        Vector p;
        rot = landed_planetoid->get_landing_position_angle(landed_location, &p);
        pos = p.to_sfml();
        sprite.setOrigin(0, sprite.getLocalBounds().height/2);
    }
    else
    {
        sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
        rot = 60*rotation;
        pos = axial_to_pixel(position.q, position.r).to_sfml();
    }

    sprite.setColor(sf::Color(50, 255, 50, 200));
    sprite.setTexture(ship_texture);
    sprite.setRotation(rot);
    sprite.setPosition(pos);
    target->draw(sprite);

    // draw next position
    if (!blink && (velocity != Hex() || taking_off))
    {
        if (debug)
            for (Hex movement_hex: (position+velocity).all_hexes_between(position))
                movement_hex.draw(target, true, sf::Color(50, 255, 50, 100));
        sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
        sprite.setColor(sf::Color(50, 255, 50, 100));
        sprite.setRotation(60*rotation);
        sprite.setTexture(ship_dashed_texture);
        sprite.setPosition(axial_to_pixel(position.q+velocity.q, position.r+velocity.r).to_sfml());
        target->draw(sprite);
    }
    elapsed_time += dt;
    if (elapsed_time > 0.5)
    {
        elapsed_time = 0;
        blink = ! blink;
    }
}

void Ship::rotate(int dirc)
{
    elapsed_time = 0;
    blink = false;
    if (dirc == 1)
        rotation = (1+rotation)%6;
    else if (dirc == -1)
        rotation = rotation==0 ? 5 : rotation-1;
    else
        assert(false && "invalid dirc argument value to Ship::rotate");
}

void Ship::accelerate(int mag)
{
    elapsed_time = 0;
    blink = false;
    if (landed && !taking_off) 
        taking_off = true;
    else
        velocity += rotate_hex(Hex(1, 0), rotation)*mag;
}