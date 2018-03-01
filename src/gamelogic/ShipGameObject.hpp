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
    int rotation;

    std::vector<Planetoid*>* planets;

    // landed state
    bool landed, taking_off;
    Planetoid* landed_planetoid;
    int landed_location;

    // flag for renderer
    bool reset_position_preview;

    // AI
    std::deque<Hex> planned_accelerations;

    ShipGameObject(Hex pos, std::vector<Planetoid*>* planets);
    void update();

    void rotate(int dirc);
    void accelerate(int mag);

    void pathfind_to(Hex goal_pos, Hex goal_vel);
};

#endif