#include "Planetoid.hpp"

Planetoid::Planetoid(int size, Hex position)
: PlanetoidGameObject(size, position), PlanetoidRenderer(this)
{}