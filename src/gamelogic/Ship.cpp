#include "Ship.hpp"

Ship::Ship(Hex pos, std::vector<Planetoid*>* planets)
: position(pos), velocity(Hex(0,0)), rotation(0), planets(planets), landed(false),
  landed_planetoid(NULL), landed_location(-1), blink(false), elapsed_time(0)
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
        landed = false;
        landed_location = -1;
        landed_planetoid = NULL;
        sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
        if (velocity == Hex()) return;
    }

    // check for landing
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

                // set drawing parameters
                sprite.setOrigin(0, sprite.getLocalBounds().height/2);
                return;
            }
        }
    }

    // check for crash, scanning all hexes that were moved through
    for (Planetoid* p : *planets)
        for (Hex movement_hex: (position-velocity).all_hexes_between(position))
            if (movement_hex.distance(p->position) <= p->size)
                for (Hex collision_hex: p->collision)
                    if ((collision_hex+p->position) == movement_hex)
                        std::cout << "crash! " << p->size << std::endl;

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
        std::cout << "gravity " << gravity << " distance: " << distance << std::endl;
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
        double theta = landed_planetoid->get_landing_position_angle(landed_location, &p);
        rot = theta*RADIANS_TO_DEGREES;
        pos = p.to_sfml();
    }
    else
    {
        rot = 60*rotation;
        pos = axial_to_pixel(position.q, position.r).to_sfml();
    }

    sprite.setColor(sf::Color(50, 255, 50, 200));
    sprite.setTexture(ship_texture);
    sprite.setRotation(rot);
    sprite.setPosition(pos);
    target->draw(sprite);


    // draw next position
    if (!blink && velocity != Hex())
    {
        if (debug)
            for (Hex movement_hex: (position+velocity).all_hexes_between(position))
                movement_hex.draw(target, true, sf::Color(50, 255, 50, 10));
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
    velocity += rotate_hex(Hex(1, 0), rotation)*mag;
}