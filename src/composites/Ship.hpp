#ifndef ship_hpp
#define ship_hpp

#include "gamelogic/ShipGameObject.hpp"
#include "rendering/ShipRenderer.hpp"

class Ship : public ShipGameObject, public ShipRenderer
{
public:
    Ship(Hex pos, std::vector<Planetoid*>* planets);
};

#endif