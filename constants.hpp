#ifndef constants_hpp
#define constants_hpp

#include <SFML/Graphics.hpp>
#include <random>

const double pi = 3.141592653589793;

const sf::Color Red = sf::Color(255, 0, 0);
const sf::Color Blue = sf::Color(0, 0, 255);
const sf::Color Green = sf::Color(0, 255, 0);
const sf::Color Yellow = sf::Color(255, 255, 0);
const sf::Color Grey = sf::Color(128, 128, 128);
const sf::Color LightGrey = sf::Color(200, 200, 200);
const sf::Color DarkGrey = sf::Color(5, 5, 5);
const sf::Color Transparent = sf::Color(0, 0, 0, 0);

const sf::Color White = sf::Color(255, 255, 255);
const sf::Color Brown = sf::Color(139, 69, 19);

const sf::Color BG_COLOUR = Yellow;

const double RADIANS_TO_DEGREES = 180/pi;
const double DEGREES_TO_RADIANS = pi/180;

const double SQRT3 = 1.7320508075688772;

const double HEX_SIZE = 100;
const double HEX_HEIGHT = 2*HEX_SIZE;
const double HEX_WIDTH = SQRT3*HEX_HEIGHT/2;

const int WIDTH = 1920;
const int HEIGHT = 1080;

extern std::default_random_engine RandomGenerator;

#endif