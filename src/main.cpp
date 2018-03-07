#include <SFML/Graphics.hpp>
#include "composites/AnimatedGameObject.hpp"
#include "composites/TurnManager.hpp"
#include "composites/Planetoid.hpp"
#include "composites/Ship.hpp"
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
    bool draw_gravity = false;

    // hacky workaround for mouse position with pixelation shader
    sf::RenderTexture window_dummy;
    window_dummy.create(window.getSize().x, window.getSize().y);


    ////////////////////////////////////////
    //          game logic                //
    ////////////////////////////////////////

    Background background;

    Planetoid planet1(1, Hex(-5, 0), "I");
    Planetoid planet2(2, Hex(10, 0), "II");
    Planetoid planet3(3, Hex(25, 0), "III");
    Planetoid planet4(4, Hex(20, -20), "IV");
    Planetoid planet5(5, Hex(-32, 32), "V");
    Planetoid planet6(6, Hex(-30, -30), "VI");

    std::vector<Planetoid*> planets;
    planets.push_back(&planet1);
    planets.push_back(&planet2);
    planets.push_back(&planet3);
    planets.push_back(&planet4);
    planets.push_back(&planet5);
    planets.push_back(&planet6);

    Ship ship(Hex(0,0), &planets);

    sf::Clock clock;
    double dt = 0;

    std::vector<AnimatedGameObject*> animated_objects;
    animated_objects.push_back(&ship);
    animated_objects.push_back(&planet1);
    animated_objects.push_back(&planet2);
    animated_objects.push_back(&planet3);
    animated_objects.push_back(&planet4);
    animated_objects.push_back(&planet5);
    animated_objects.push_back(&planet6);

    TurnManager turn_manager(animated_objects);

    ////////////////////////////////////////
    //          shader & vfx variables    //
    ////////////////////////////////////////

    Bloom bloom;
    Flicker flicker;
    float pixelation_factor = 1.0;

    sf::RenderTexture working_texture1, working_texture2;
    working_texture1.create(window.getSize().x/pixelation_factor, window.getSize().y/pixelation_factor);
    working_texture1.setSmooth(true);
    working_texture2.create(window.getSize().x/pixelation_factor, window.getSize().y/pixelation_factor);
    working_texture2.setSmooth(true);

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
                view.move(-10*zoom, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                view.move(10*zoom, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                view.move(0, -10*zoom);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                view.move(0, 10*zoom);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                view.zoom(1.1);
                zoom *= 1.1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
                if (zoom > 1)
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
                    if (event.key.code == sf::Keyboard::Subtract)
                    {
                        if (ship.PATH_ANIMATE_TIME > 1)
                        {
                            ship.PATH_ANIMATE_TIME -= 0.5;
                            turn_manager.TOTAL_TURN_TIME -= 0.5;
                        }
                    }
                    if (event.key.code == sf::Keyboard::Add)
                    {
                        ship.PATH_ANIMATE_TIME += 0.5;
                        turn_manager.TOTAL_TURN_TIME += 0.5;
                    }
                    if (event.key.code == sf::Keyboard::C)
                        hex_nums = !hex_nums;
                    if (event.key.code == sf::Keyboard::X)
                        debug = !debug;
                    if (event.key.code == sf::Keyboard::Z)
                    {
                        shaders_on = !shaders_on;
                        if (!shaders_on)
                        {
                            working_texture1.create(window.getSize().x, window.getSize().y);
                            working_texture1.setSmooth(true);
                            working_texture2.create(window.getSize().x, window.getSize().y);
                            working_texture2.setSmooth(true);
                            pixelation_factor = 1;
                        }
                    }
                    if (event.key.code == sf::Keyboard::Space)
                        turn_manager.paused = !turn_manager.paused;
                    if (event.key.code == sf::Keyboard::Q)
                    {
                        if (ship.planned_accelerations.size() > 0)
                            ship.cancel_pathfinding();
                        else
                            ship.velocity *= 0;
                    }
                    if (event.key.code == sf::Keyboard::V)
                        draw_gravity = !draw_gravity;
                }
                if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        window_dummy.setView(view);
                        sf::Vector2f mouse_pos = window_dummy.mapPixelToCoords(sf::Mouse::getPosition(window));
                        Hex h = Hex(pixel_to_axial(mouse_pos.x, mouse_pos.y));
                        ship.take_path_input(h);
                    }
            }
        }

        ////////////////////////////////////////
        //            game logic              //
        ////////////////////////////////////////  
        {
            turn_manager.update(dt);
        }
        ////////////////////////////////////////
        //            rendering               //
        ////////////////////////////////////////  
        {
            // shader init
            if (shaders_on)
            {
                float prev_factor = pixelation_factor;
                pixelation_factor = flicker.get_pixelation_factor();
                if (pixelation_factor != prev_factor)
                {
                    working_texture1.create(window.getSize().x/pixelation_factor, window.getSize().y/pixelation_factor);
                    working_texture1.setSmooth(true);
                    working_texture2.create(window.getSize().x/pixelation_factor, window.getSize().y/pixelation_factor);
                    working_texture2.setSmooth(true);
                }
            }

            // draw entities
            working_texture1.clear(BG_COLOUR);
            working_texture1.setView(view);
            background.draw(&working_texture1, zoom, hex_nums);
            for (Planetoid* p: planets)
                p->draw(&working_texture1, dt, draw_gravity, debug);

            window_dummy.setView(view);
            sf::Vector2f mouse_pos = window_dummy.mapPixelToCoords(sf::Mouse::getPosition(window));
            Hex mouse_hex = Hex(pixel_to_axial(mouse_pos.x, mouse_pos.y));

            ship.draw(&working_texture1, dt, mouse_hex, debug);
            turn_manager.draw(&working_texture1);

            // Debugging: hex selection
            mouse_hex.draw(&working_texture1, false, Green);
            
            // draw to window
            working_texture1.display();
            if (shaders_on)
            {
                bloom.apply(working_texture1, working_texture2);
                flicker.apply(working_texture2, window);
            }
            else
            {
                sf::Sprite sprite(working_texture1.getTexture());
                window.draw(sprite);
            }
            window.display();
        }

        dt = clock.restart().asSeconds();
    }
    return 0;
}
