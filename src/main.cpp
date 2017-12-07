#include <SFML/Graphics.hpp>
#include "gamelogic/Planetoid.hpp"
#include "gamelogic/Ship.hpp"
#include "rendering/Bloom.hpp"
#include "rendering/Flicker.hpp"
#include "rendering/Background.hpp"
#include "utilities/Hex.hpp"
#include "utilities/constants.hpp"
#include "utilities/utilities.hpp"

int main(int argc, char* argv[])
{   
    ////////////////////////////////////////
    //          SFML boilerplate          //
    ////////////////////////////////////////
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "HIDE(N)", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::View view = window.getDefaultView();

    ////////////////////////////////////////
    //          control variables         //
    ////////////////////////////////////////

    double zoom = 1.0;
    bool shaders_on = true;

    ////////////////////////////////////////
    //          game logic                //
    ////////////////////////////////////////

    Background background;

    Planetoid planet1(2, Hex(0, 0));

    std::vector<Planetoid*> planets;
    planets.push_back(&planet1);

    Ship ship(Hex(5,0), &planets);

    ////////////////////////////////////////
    //          shader & vfx variables    //
    ////////////////////////////////////////

    Bloom bloom;
    Flicker flicker;

    sf::RenderTexture canvas, output_canvas;
    canvas.create(window.getSize().x, window.getSize().y);
    output_canvas.create(window.getSize().x, window.getSize().y);
    
    ////////////////////////////////////////
    //               main loop            //
    ////////////////////////////////////////
    while (window.isOpen())
    {
        ////////////////////////////////////////
        //          event handling            //
        ////////////////////////////////////////
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
                if (zoom > 1)
                {
                    view.zoom(0.9);
                    zoom *= 0.9;
                }
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
                    if (event.key.code == sf::Keyboard::Z)
                        shaders_on = !shaders_on;
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        ship.update();
                    }
                }
            }
        }
        ////////////////////////////////////////
        //            rendering               //
        ////////////////////////////////////////  
        {
            canvas.clear(BG_COLOUR);
            canvas.setView(view);
            background.draw(&canvas, zoom);
            planet1.draw(&canvas);
            ship.draw(&canvas);
            canvas.display();

            if (shaders_on)
            {
                bloom.apply(canvas, output_canvas);
                flicker.apply(output_canvas, window);
            }
            else
            {
                sf::Sprite sprite(canvas.getTexture());
                window.draw(sprite);
            }
            window.display();
        }
    }
    return 0;
}
