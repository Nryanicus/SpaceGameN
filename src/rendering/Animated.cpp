#include "Animated.hpp"

Animated::Animated()
: elapsed_time_animate(0), animation_state(AnimationState::None)
{}

void Animated::begin_animating()
{
    if (animation_state == AnimationState::None)
        animation_state = AnimationState::Animating;
}

bool Animated::get_done_animating()
{
    return (animation_state == AnimationState::Done);
}

void Animated::update_animation(double dt)
{
    if (animation_state == AnimationState::Animating)
    {
        elapsed_time_animate += dt;
        if (elapsed_time_animate > TOTAL_TIME_ANIMATE)
            animation_state = AnimationState::Done;
    }
}

void Animated::finalise_animation()
{
    elapsed_time_animate = 0;
    animation_state = AnimationState::None;
}