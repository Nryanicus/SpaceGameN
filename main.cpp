#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Hex.hpp"
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

    window.setView(view);

    Background background;

    sf::Font arrow_font;
    arrow_font.loadFromFile("res/LiberationMono-Regular.ttf");

    sf::Clock clock;
    float time = 0;

    double zoom = 1.0;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            view.move(-1*zoom, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            view.move(1*zoom, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            view.move(0, -1*zoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            view.move(0, 1*zoom);
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
                if (event.key.code == sf::Keyboard::Space)
                {
                    // ship.update();
                }
            }
        }
        // clear the window with black color
        background.clear_all_hexes();

        window.setView(view);
        window.clear(DarkGrey);

        background.draw(&window, &arrow_font, zoom);
        // ship.draw(&window, &opts, true);

        // end the current frame
        window.display();
        time = clock.restart().asSeconds();
    }
    return 0;
}
