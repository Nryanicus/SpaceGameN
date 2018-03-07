#ifndef shipgameobject_hpp
#define shipgameobject_hpp

#include <SFML/Graphics.hpp>
#include <future>
#include <chrono>
#include "PlanetoidGameObject.hpp"
#include "pathfinder.hpp"
#include "utilities/Hex.hpp"
#include "composites/Planetoid.hpp"

const std::chrono::milliseconds NO_TIME(0);

enum class PathfindingState {None, Computing, PathFound, PathNotFound, Waiting};

class ShipGameObject
{
private:
    void check_pathfinding();
    bool is_in_orbit();
public:
    // game state
    Hex acceleration;
    Hex position;
    Hex velocity;
    int rotation;

    std::vector<Planetoid*>* planets;

    // landed state
    bool landed, taking_off;
    Planetoid* landed_planetoid;
    int landed_location;

    // flags for renderer
    bool reset_position_preview;

    // AI
    bool pathfind_from_orbit;
    Hex orbital_start_pos;
    PathfindingState pathfinding_state;
    std::future<std::deque<Hex>> pathfinder_return;
    std::deque<Hex> planned_accelerations;

    ShipGameObject(Hex pos, std::vector<Planetoid*>* planets);
    void update();

    void rotate(int dirc);
    void accelerate(int mag);

    void pathfind_to(Hex goal_pos, Hex goal_vel);
    void cancel_pathfinding();

};

#endif