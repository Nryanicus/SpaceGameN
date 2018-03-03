#include "TurnManager.hpp"

TurnManager::TurnManager(std::vector<AnimatedGameObject*> objects)
: animation_manager(AnimationManager(objects)),
  turn_timer(0), TOTAL_TURN_TIME(2),
  paused(false)
{
    clock_array.setPrimitiveType(sf::PrimitiveType::LineStrip);
}

void TurnManager::update(double dt)
{
    animation_manager.update(dt);

    if (paused) return;
    
    turn_timer += dt;
    if (turn_timer > TOTAL_TURN_TIME)
    {
        turn_timer = 0;
        animation_manager.begin();
    }

    int last_seg = ceil(CLOCK_SEGMENTS - CLOCK_SEGMENTS*turn_timer/TOTAL_TURN_TIME);
    bool full_circle = last_seg==CLOCK_SEGMENTS;
    int array_size = full_circle ? CLOCK_SEGMENTS : last_seg+2;
    clock_array.resize(array_size);
    int i = full_circle ? 0 : 1;
    int j = full_circle ? 0 : 1;

    for (; i<last_seg; i++)
    {
        double r = ((double)i-j)/((double)CLOCK_SEGMENTS);
        clock_array[i] = sf::Vertex( (CLOCK_RADIUS*Vector(0,-1).rotate(r*2*pi)).to_sfml(), CLOCK_COLOUR);
    }
    if (!full_circle)
    {
        clock_array[0] = sf::Vertex(Vector(0,0).to_sfml(), CLOCK_COLOUR);
        clock_array[array_size-1] = sf::Vertex(Vector(0,0).to_sfml(), CLOCK_COLOUR);
        double r = ((double)(CLOCK_SEGMENTS - CLOCK_SEGMENTS*turn_timer/TOTAL_TURN_TIME))/((double)CLOCK_SEGMENTS);
        clock_array[array_size-2] = sf::Vertex( (CLOCK_RADIUS*Vector(0,-1).rotate(r*2*pi)).to_sfml(), CLOCK_COLOUR);
    }
}

void TurnManager::draw(sf::RenderTarget* target)
{
    sf::Transform trans = target->getView().getInverseTransform();
    trans.translate(CLOCK_POS.to_sfml());
    trans.scale(CLOCK_SCALE.to_sfml());

    target->draw(clock_array, sf::RenderStates(trans));
}