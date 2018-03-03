#ifndef turnmanager_hpp
#define turnmanager_hpp

#include <SFML/Graphics.hpp>
#include "AnimationManager.hpp"
#include "utilities/Vector.hpp"
#include "utilities/constants.hpp"
#include <iostream>

const double CLOCK_SEGMENTS = 32;
const double CLOCK_RADIUS = 0.1;
const sf::Color CLOCK_COLOUR(255, 255, 255, 255);

const Vector CLOCK_POS(0.9,-0.9);
const Vector CLOCK_SCALE(-((double)HEIGHT)/((double)WIDTH),-1.0);

class TurnManager
{
private:
    AnimationManager animation_manager;
    double turn_timer = 0;
    double TOTAL_TURN_TIME = 2;

    sf::VertexArray clock_array;
public:
    bool paused;
    TurnManager(std::vector<AnimatedGameObject*> objects);
    void update(double dt);
    void draw(sf::RenderTarget* target);
};

#endif