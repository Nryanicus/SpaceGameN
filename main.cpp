#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Hex.hpp"
#include "Planetoid.hpp"
#include "Ship.hpp"
#include "Background.hpp"
#include "constants.hpp"
#include "utilities.hpp"

int main(int argc, char* argv[])
{   
    // create the window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "HIDE(N)", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::View view = window.getDefaultView();

    Background background;

    sf::Font arrow_font;
    arrow_font.loadFromFile("res/LiberationMono-Regular.ttf");

    double zoom = 1.0;

    Planetoid planet1(6, Hex(0, 0));

    std::vector<Planetoid*> planets;
    planets.push_back(&planet1);

    sf::Texture ship_tex;
    ship_tex.loadFromFile("res/ship.png");
    ship_tex.setSmooth(true);
    Ship ship(Hex(5,0), &ship_tex, &planets);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // Event handling
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                view.move(-10*zoom, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                view.move(10*zoom, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                view.move(0, -10*zoom);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                view.move(0, 10*zoom);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                view.zoom(1.1);
                zoom *= 1.1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            {
                view.zoom(0.9);
                zoom *= 0.9;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Left)
                        ship.rotate(-1);
                    if (event.key.code == sf::Keyboard::Right)
                        ship.rotate(1);
                    if (event.key.code == sf::Keyboard::Up)
                        ship.accelerate(1);
                    if (event.key.code == sf::Keyboard::Down)
                        ship.accelerate(-1);
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        ship.update();
                    }
                }
            }
        }
        
        // Rendering
        {
            // clear the window with black color
            window.setView(view);
            window.clear(DarkGrey);

            background.draw(&window, &arrow_font, zoom);
            planet1.draw(&window);
            ship.draw(&window);

            // end the current frame
            window.display();
        }
    }
    return 0;
}
