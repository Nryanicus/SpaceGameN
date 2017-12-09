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
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "HIDE(N)", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::View view = window.getDefaultView();

    ////////////////////////////////////////
    //          control variables         //
    ////////////////////////////////////////

    double zoom = 1.0;
    bool shaders_on = false;
    bool debug = false;
    bool hex_nums = false;

    ////////////////////////////////////////
    //          game logic                //
    ////////////////////////////////////////

    Background background;

    Planetoid planet1(1, Hex(0, 0));
    Planetoid planet2(2, Hex(10, 0));
    Planetoid planet3(3, Hex(25, 0));
    // Planetoid planet4(4, Hex(20, 0));
    // Planetoid planet5(5, Hex(32, 0));
    // Planetoid planet6(6, Hex(45, 0));

    std::vector<Planetoid*> planets;
    planets.push_back(&planet1);
    planets.push_back(&planet2);
    planets.push_back(&planet3);
    // planets.push_back(&planet4);
    // planets.push_back(&planet5);
    // planets.push_back(&planet6);

    Ship ship(Hex(5,0), &planets);

    sf::Clock clock;
    double dt = 0;

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
                    if (event.key.code == sf::Keyboard::C)
                        hex_nums = !hex_nums;
                    if (event.key.code == sf::Keyboard::X)
                        debug = !debug;
                    if (event.key.code == sf::Keyboard::Z)
                        shaders_on = !shaders_on;
                    if (event.key.code == sf::Keyboard::Space)
                        ship.update();
                }
            }
        }
        ////////////////////////////////////////
        //            rendering               //
        ////////////////////////////////////////  
        {
            canvas.clear(BG_COLOUR);
            canvas.setView(view);
            background.draw(&canvas, zoom, hex_nums);
            for (Planetoid* p: planets)
                p->draw(&canvas, debug);
            ship.draw(&canvas, dt, debug);
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

        dt = clock.restart().asSeconds();
    }
    return 0;
}
