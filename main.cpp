#include <SFML/Graphics.hpp>
#include "Hex.hpp"
#include "Planetoid.hpp"
#include "Ship.hpp"
#include "Background.hpp"
#include "constants.hpp"
#include "utilities.hpp"

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

    sf::Shader gauss_shader;
    gauss_shader.loadFromFile("shaders/gauss.frag", sf::Shader::Fragment);
    gauss_shader.setUniform("texture", sf::Shader::CurrentTexture);
    gauss_shader.setUniform("resolution", sf::Vector2f(window.getSize().x, window.getSize().y));
    
    sf::RenderTexture canvas, bg_canvas;
    canvas.create(window.getSize().x, window.getSize().y);
    bg_canvas.create(window.getSize().x, window.getSize().y);
    
    int gauss_rot = 0;
    float gauss_mag = 1;
    sf::Clock clock;
    float time = 0;
    float time_uniform = 0;
    bool no_canvas_clear = false;

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
                no_canvas_clear = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            {
                if (zoom > 1)
                {
                    view.zoom(0.9);
                    zoom *= 0.9;
                    no_canvas_clear = true;
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
        //          rendering logic           //
        ////////////////////////////////////////        
        {
            gauss_rot +=random_int(-3,3);
            gauss_mag += ((float)random_int(-100,100))/10000.0f;
            if (gauss_mag < 0) gauss_mag = 1;
            if (gauss_mag > 2) gauss_mag = 2;

            gauss_shader.setUniform("seed", ((float)random_int(0,100000))/100000.0f);
            gauss_shader.setUniform("direction", gauss_mag*Vector(1,0).rotate(gauss_rot*DEGREES_TO_RADIANS).to_sfml());
            gauss_shader.setUniform("time", time_uniform);
            time = clock.restart().asSeconds();
            time_uniform += time;
            if (time_uniform > 31415) // pi*10000, to prevent overflows
                time_uniform = 0;
            if (!no_canvas_clear)
                no_canvas_clear = !random_int(0,3);
        }
        ////////////////////////////////////////
        //            rendering               //
        ////////////////////////////////////////  
        {
            bg_canvas.clear(BG_COLOUR);
            bg_canvas.setView(view);
            background.draw(&bg_canvas, zoom);
            bg_canvas.display();

            if (!no_canvas_clear || !shaders_on)
                canvas.clear(Transparent);
            canvas.setView(view);
            planet1.draw(&canvas);
            ship.draw(&canvas);
            canvas.display();

            sf::Sprite bg_sprite(bg_canvas.getTexture());
            sf::Sprite gauss_sprite(canvas.getTexture());

            if (shaders_on)
            {
                window.draw(bg_sprite, &gauss_shader);
                window.draw(gauss_sprite, &gauss_shader);
            }
            else
            {
                window.draw(bg_sprite);
                window.draw(gauss_sprite);
            }
            window.display();
            no_canvas_clear = false;
        }
    }
    return 0;
}
