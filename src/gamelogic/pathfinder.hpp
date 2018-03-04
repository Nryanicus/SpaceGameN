#ifndef pathfinder_hpp
#define pathfinder_hpp

#include <unordered_map>
#include <queue>
#include "PlanetoidGameObject.hpp"
#include "utilities/Hex.hpp"

const int BEAM_WIDTH = 10000;
const int MAX_SEARCH = 50000;
const int BEAM_SEARCH_CUTOFF = 10;

std::deque<Hex> pathfind(Hex start_pos, Hex start_vel, Hex goal_pos, Hex goal_vel, int acceleration, std::vector<PlanetoidGameObject*>* planetoids);

#endif