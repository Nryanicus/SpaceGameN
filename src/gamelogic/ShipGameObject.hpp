#ifndef shipgameobject_hpp
#define shipgameobject_hpp

#include <SFML/Graphics.hpp>
#include "pathfinder.hpp"
#include "utilities/Hex.hpp"
#include "composites/Planetoid.hpp"

class ShipGameObject
{
public:
    // game state
    Hex position;
    Hex velocity;

    std::vector<Planetoid*>* planets;

    // landed state
    bool landed, taking_off;
    Planetoid* landed_planetoid;
    int landed_location;

    // flags for renderer
    bool reset_position_preview;
    int accelerating;

    // AI
    std::deque<Hex> planned_accelerations;

    ShipGameObject(Hex pos, std::vector<Planetoid*>* planets);
    void update();

    void pathfind_to(Hex goal_pos, Hex goal_vel);
};

#endif