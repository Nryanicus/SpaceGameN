#include "Flicker.hpp"

Flicker::Flicker()
: PostEffect(),
  noise_proportion(0.05), smear_magnitude(1.0), smear_direction(0), elasped_time(0),
  lurch(1)
{
}

void Flicker::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
    elasped_time += clock.restart().asSeconds();
    if (elasped_time > 31415) elasped_time = 0;
    prepare_textures(input.getSize());

    lurch += ((float)random_int(-100,100))/1000.0;
    if (lurch < 0.5) lurch = 0.5;
    if (lurch > 1.5) lurch = 1.5;

    // Smear
    smear_direction += random_int(-10,10);
    smear_magnitude += sin(elasped_time)/2 + 0.5;
    smear_magnitude *= lurch;

    if (smear_magnitude <= 0) smear_magnitude = 0;
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
    noise_proportion = cos(elasped_time)/20+0.05;
    noise_proportion /= lurch;
    if (noise_proportion <= 0) noise_proportion = 0;
    if (noise_proportion > 0.1) noise_proportion = 0.1;
    float noise_uniform = noise_proportion/4 + smear_magnitude/80;

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

float Flicker::get_pixelation_factor()
{
    int factor = round(sin(elasped_time/10)*1.5+1.5);
    if (factor < 1) factor = 1;
    return factor;
}
