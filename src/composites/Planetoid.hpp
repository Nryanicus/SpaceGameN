#ifndef planetoid_hpp
#define planetoid_hpp

#include "gamelogic/PlanetoidGameObject.hpp"
#include "rendering/PlanetoidRenderer.hpp"
#include "AnimatedGameObject.hpp"

class Planetoid : public PlanetoidGameObject, public PlanetoidRenderer, public AnimatedGameObject
{
public:
    Planetoid(int size, Hex position, std::string name);
    virtual void begin_animating();
    virtual bool get_done_animating();
    virtual void update_animation(double dt);
    virtual void finalise_animation();
    virtual void update();
};

#endif