#include "Ship.hpp"

Ship::Ship(Hex pos, std::vector<Planetoid*>* planets)
: ShipGameObject(pos, planets), ShipRenderer(this)
{}