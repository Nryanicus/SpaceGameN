#ifndef ship_hpp
#define ship_hpp

#include "gamelogic/ShipGameObject.hpp"
#include "rendering/ShipRenderer.hpp"
#include "AnimatedGameObject.hpp"

class Ship : public ShipGameObject, public ShipRenderer, public AnimatedGameObject
{
public:
    Ship(Hex pos, std::vector<Planetoid*>* planets);
    virtual void begin_animating();
    virtual bool get_done_animating();
    virtual void update_animation(double dt);
    virtual void finalise_animation();
    virtual void update();
};

#endif