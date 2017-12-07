#ifndef flicker_hpp
#define flicker_hpp

#include <SFML/Graphics.hpp>
#include "Vector.hpp"
#include "utilities.hpp"

class Flicker
{
private:
    sf::Clock clock;
    float noise_proportion, smear_magnitude;
    int smear_direction; // in degrees
    float elasped_time;

    void prepare_textures(sf::Vector2u size);

    void smear(const sf::RenderTexture& input, sf::RenderTexture& output);
    void noise(const sf::RenderTexture& input, sf::RenderTarget& target);

    static void apply_shader(const sf::Shader& shader, sf::RenderTarget& output);

public:
    Flicker();
    void apply(const sf::RenderTexture& input, sf::RenderTarget& output);

    sf::Shader noise_shader, smear_shader;

    sf::RenderTexture working_texture;
};

#endif