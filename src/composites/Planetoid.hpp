#ifndef planetoid_hpp
#define planetoid_hpp

#include "gamelogic/PlanetoidGameObject.hpp"
#include "rendering/PlanetoidRenderer.hpp"

class Planetoid : public PlanetoidGameObject, public PlanetoidRenderer
{
public:
    Planetoid(int size, Hex position);
};

#endif