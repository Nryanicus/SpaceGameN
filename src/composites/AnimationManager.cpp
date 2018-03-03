#include "AnimationManager.hpp"

AnimationManager::AnimationManager(std::vector<AnimatedGameObject*> objects)
: objects(objects)
{}


void AnimationManager::begin()
{
    for (AnimatedGameObject* o: objects)
        o->begin_animating();
}

void AnimationManager::update(double dt)
{
    bool all_done = true;
    for (AnimatedGameObject* o: objects)
    {
        o->update_animation(dt);
        all_done = all_done && o->get_done_animating();
    }
    if (all_done)
        for (AnimatedGameObject* o: objects)
        {
            o->finalise_animation();
            o->update();
        }
}
