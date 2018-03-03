#include "Ship.hpp"

Ship::Ship(Hex pos, std::vector<Planetoid*>* planets)
: ShipGameObject(pos, planets), ShipRenderer(this)
{}
    
void Ship::begin_animating()
{
    ShipRenderer::begin_animating();
}

bool Ship::get_done_animating()
{
    return ShipRenderer::get_done_animating();
}

void Ship::update_animation(double dt)
{
    ShipRenderer::update_animation(dt);
}

void Ship::finalise_animation()
{
    ShipRenderer::finalise_animation();
}

void Ship::update()
{
    ShipGameObject::update();
}