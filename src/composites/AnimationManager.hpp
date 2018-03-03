#ifndef animationmanager_hpp
#define animationmanager_hpp

#include "AnimatedGameObject.hpp"
#include <vector>

class AnimationManager
{
    std::vector<AnimatedGameObject*> objects;
public:
    AnimationManager(std::vector<AnimatedGameObject*> objects);
    void update(double dt);
    void begin();
};

#endif
