#include "Ship.hpp"

Ship::Ship(Hex pos, sf::Texture* tex, std::vector<Planetoid*>* planets)
: position(pos), velocity(Hex(0,0)), rotation(0), planets(planets)
{
    sprite.setTexture(*tex);
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
}

void Ship::update()
{
    position += velocity;

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
    sprite.setColor(White);
    sprite.setRotation(60*rotation);
    sprite.setPosition(axial_to_pixel(position.q, position.r).to_sfml());
    target->draw(sprite);

    // draw next position
    sprite.setColor(sf::Color(0, 255, 0, 100));
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