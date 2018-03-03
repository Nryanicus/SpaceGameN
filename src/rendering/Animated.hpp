#ifndef animated_hpp
#define animated_hpp

const double TOTAL_TIME_ANIMATE = 1;

enum class AnimationState {None, Animating, Done};

class Animated
{
protected:
    double elapsed_time_animate;
    AnimationState animation_state;
public:
    Animated();
    void begin_animating();
    bool get_done_animating();
    void update_animation(double dt);
    void finalise_animation();
};

#endif
