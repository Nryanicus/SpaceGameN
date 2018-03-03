#include "Planetoid.hpp"

Planetoid::Planetoid(int size, Hex position, std::string name)
: PlanetoidGameObject(size, position, name), PlanetoidRenderer(this)
{}

void Planetoid::begin_animating()
{
    PlanetoidRenderer::begin_animating();
}

bool Planetoid::get_done_animating()
{
    return PlanetoidRenderer::get_done_animating();
}

void Planetoid::update_animation(double dt)
{
    PlanetoidRenderer::update_animation(dt);
}

void Planetoid::finalise_animation()
{
    PlanetoidRenderer::finalise_animation();
}

void Planetoid::update()
{
    PlanetoidGameObject::update();
}