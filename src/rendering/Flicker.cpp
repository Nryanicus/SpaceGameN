#include "Flicker.hpp"

Flicker::Flicker()
: PostEffect(),
  noise_proportion(0.05), smear_magnitude(1.0), smear_direction(0), elasped_time(0)
{
}

void Flicker::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
    elasped_time += clock.restart().asSeconds();
    if (elasped_time > 31415) elasped_time = 0;
    prepare_textures(input.getSize());


    // Smear
    smear_direction += random_int(-10,10);
    smear_magnitude += (sin(elasped_time*((float)random_int(-100,100))/1000.0f)/2);

    if (smear_magnitude < 0) smear_magnitude = 0;
    if (smear_magnitude > 2) smear_magnitude = 2;
    sf::Vector2f smear_vector = (2*smear_magnitude*Vector(1,0).rotate(smear_direction*DEGREES_TO_RADIANS)).to_sfml();
    smear(input, working_texture, smear_vector);

    // Aberrate
    sf::Vector2f r_off = smear_magnitude*Vector(0.002,0.0).rotate(smear_direction*DEGREES_TO_RADIANS).to_sfml();
    sf::Vector2f g_off = smear_magnitude*Vector(0.001,0.0).rotate(smear_direction*DEGREES_TO_RADIANS).to_sfml();
    sf::Vector2f b_off = smear_magnitude*Vector(-0.002,0.0).rotate(smear_direction*DEGREES_TO_RADIANS).to_sfml();
    aberrate(working_texture, working_texture2, r_off, g_off, b_off);
    add(input, working_texture2, working_texture);

    // noise
    noise_proportion += ((float)random_int(-100000, 100000))/100000000.0f;
    if (noise_proportion < 0) noise_proportion = 0;
    if (noise_proportion > 0.1) noise_proportion = 0.1;
    float noise_uniform = noise_proportion + smear_magnitude/40;

    noise(working_texture, working_texture2, noise_uniform);

    apply_to_target(working_texture2, output);
}

void Flicker::prepare_textures(sf::Vector2u size)
{
    if (working_texture.getSize() != size)
    {
        noise_shader.setUniform("resolution", sf::Vector2f(size.x, size.y));
        smear_shader.setUniform("resolution", sf::Vector2f(size.x, size.y));
        abberation_shader.setUniform("resolution", sf::Vector2f(size.x, size.y));
        working_texture.create(size.x, size.y);
        working_texture.setSmooth(true);
        working_texture2.create(size.x, size.y);
        working_texture2.setSmooth(true);
    }
}