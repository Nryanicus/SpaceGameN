#include "Ship.hpp"

Ship::Ship(Hex pos, std::vector<Planetoid*>* planets)
: position(pos), velocity(Hex(0,0)), rotation(0), planets(planets), landed(false),
  landed_planetoid(NULL)
{
    ship_texture.loadFromFile("res/sprites/ship.png");
    ship_texture.setSmooth(true);
    ship_dashed_texture.loadFromFile("res/sprites/ship_dashed.png");
    ship_dashed_texture.setSmooth(true);
    sprite.setTexture(ship_texture);
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
    sprite.scale(0.5,0.5);
}

void Ship::update()
{
    position += velocity;
    // check for landing
    for (Planetoid* p : *planets)
    {
        int distance = position.distance(p->position);
        std::cout << distance << " " << p->size-1 << std::endl;
        if (distance < p->size)
        {
            if (distance == p->size-1 && velocity.magnitude()<=1)
            {
                std::cout << "Landing!" << std::endl;
                landed = true;
                landed_planetoid = p;
                landed_location = p->find_landing_location(position, position-velocity);
            }
        }
    }

    // check for gravity
    for (Planetoid* p : *planets)
    {
        int distance = position.distance(p->position);
        int gravity = p->mass - distance;
        // if too far away, do nothing
        if (gravity <= 0) continue;
        // otherwise accelerate towards planet
        Hex dirc = position.all_hexes_between(p->position)[1] - position;
        std::cout << distance << std::endl;
        std::cout << "gravity " << gravity << " " << dirc << std::endl;
        velocity += gravity*dirc;
    }
}

void Ship::draw(sf::RenderTarget* target)
{
    // draw current position
    sprite.setColor(sf::Color(0, 255, 0, 200));
    sprite.setTexture(ship_texture);
    sprite.setRotation(60*rotation);
    sprite.setPosition(axial_to_pixel(position.q, position.r).to_sfml());
    target->draw(sprite);

    // draw next position
    sprite.setColor(sf::Color(0, 255, 0, 100));
    sprite.setTexture(ship_dashed_texture);
    sprite.setPosition(axial_to_pixel(position.q+velocity.q, position.r+velocity.r).to_sfml());
    target->draw(sprite);
}

void Ship::rotate(int dirc)
{
    if (dirc == 1)
        rotation = (1+rotation)%6;
    else if (dirc == -1)
        rotation = rotation==0 ? 5 : rotation-1;
    else
        assert(false && "invalid dirc argument value to Ship::rotate");
}

void Ship::accelerate(int mag)
{
    velocity += rotate_hex(Hex(1, 0), rotation)*mag;
}