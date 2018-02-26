#include "Planetoid.hpp"

Planetoid::Planetoid(int size, Hex position, std::string name)
: PlanetoidGameObject(size, position, name), PlanetoidRenderer(this)
{}