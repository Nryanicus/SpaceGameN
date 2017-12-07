#include "Flicker.hpp"

Flicker::Flicker()
: noise_proportion(0.1), smear_magnitude(1.0), smear_direction(0), elasped_time(0)
{
    noise_shader.loadFromFile("../res/shaders/noise.frag", sf::Shader::Fragment);
    smear_shader.loadFromFile("../res/shaders/smear.frag", sf::Shader::Fragment);
}

void Flicker::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
    elasped_time += clock.restart().asSeconds();
    if (elasped_time > 31415) elasped_time = 0;
    prepare_textures(input.getSize());

    smear(input, working_texture);
    noise(working_texture, output);
}

void Flicker::prepare_textures(sf::Vector2u size)
{
    if (working_texture.getSize() != size)
    {
        noise_shader.setUniform("resolution", sf::Vector2f(size.x, size.y));
        smear_shader.setUniform("resolution", sf::Vector2f(size.x, size.y));
        working_texture.create(size.x, size.y);
        working_texture.setSmooth(true);
    }
}

void Flicker::smear(const sf::RenderTexture& input, sf::RenderTexture& output)
{
    smear_direction += random_int(-10,10);
    smear_magnitude += (sin(elasped_time*((float)random_int(-100,100))/1000.0f)/2);

    if (smear_magnitude < 0) smear_magnitude = 0;
    if (smear_magnitude > 2) smear_magnitude = 2;

    smear_shader.setUniform("source", input.getTexture());
    smear_shader.setUniform("direction", smear_magnitude*Vector(1,0).rotate(smear_direction*DEGREES_TO_RADIANS).to_sfml());
    apply_shader(smear_shader, output);
    output.display();
}

void Flicker::noise(const sf::RenderTexture& input, sf::RenderTarget& output)
{
    noise_shader.setUniform("source", input.getTexture());
    noise_shader.setUniform("rand", ((float)random_int(0,100000))/100000.0f);
    noise_proportion += ((float)random_int(-100000, 100000))/1000000000.0f;
    if (noise_proportion < 0) noise_proportion = 0;
    if (noise_proportion > 0.05) noise_proportion = 0.05;
    noise_shader.setUniform("noiseProportion", noise_proportion);
    apply_shader(noise_shader, output);
}

void Flicker::apply_shader(const sf::Shader& shader, sf::RenderTarget& output)
{
    sf::Vector2f outputSize = static_cast<sf::Vector2f>(output.getSize());

    sf::VertexArray vertices(sf::TrianglesStrip, 4);
    vertices[0] = sf::Vertex(sf::Vector2f(0, 0),            sf::Vector2f(0, 1));
    vertices[1] = sf::Vertex(sf::Vector2f(outputSize.x, 0), sf::Vector2f(1, 1));
    vertices[2] = sf::Vertex(sf::Vector2f(0, outputSize.y), sf::Vector2f(0, 0));
    vertices[3] = sf::Vertex(sf::Vector2f(outputSize),      sf::Vector2f(1, 0));

    sf::RenderStates states;
    states.shader    = &shader;
    states.blendMode = sf::BlendNone;

    output.draw(vertices, states);
}
