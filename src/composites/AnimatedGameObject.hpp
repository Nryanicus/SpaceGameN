#ifndef animatedgameobject_hpp
#define animatedgameobject_hpp

class AnimatedGameObject
{
public:
    virtual void begin_animating()=0;
    virtual bool get_done_animating()=0;
    virtual void update_animation(double dt)=0;
    virtual void finalise_animation()=0;
    virtual void update()=0;
};

#endif
