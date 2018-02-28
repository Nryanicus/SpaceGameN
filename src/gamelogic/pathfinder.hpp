#ifndef pathfinder_hpp
#define pathfinder_hpp

#include <unordered_map>
#include <queue>
#include "composites/Planetoid.hpp"
#include "utilities/Hex.hpp"

std::vector<Hex> pathfind(Hex start_pos, Hex start_vel, Hex goal_pos, Hex goal_vel, int max_acceleration, std::vector<Planetoid*> planetoids);

#endif