#ifndef constants_hpp
#define constants_hpp

#include <SFML/Graphics.hpp>
#include <random>

const float pi = 3.141592653589793;

const sf::Color Red = sf::Color(255, 0, 0);
const sf::Color Blue = sf::Color(0, 0, 255);
const sf::Color Green = sf::Color(0, 255, 0);
const sf::Color Yellow = sf::Color(128, 128, 0);
const sf::Color Grey = sf::Color(128, 128, 128);
const sf::Color LightGrey = sf::Color(200, 200, 200);
const sf::Color DarkGrey = sf::Color(5, 5, 5);
const sf::Color Transparent = sf::Color(0, 0, 0, 0);

const sf::Color White = sf::Color(255, 255, 255);
const sf::Color Brown = sf::Color(139, 69, 19);

const sf::Color BG_COLOUR = Yellow;


const float PIXELS_TO_METRES = 0.2;
const float METRES_TO_PIXELS = 5.0;

const float RADIANS_TO_DEGREES = 180/pi;
const float DEGREES_TO_RADIANS = pi/180;

const float SQRT3 = 1.7320508075688772;

const float HEX_HEIGHT = 2;
const float HEX_WIDTH = SQRT3;

const int WIDTH = 1000;
const int HEIGHT = 1000;

extern std::default_random_engine RandomGenerator;

#endif