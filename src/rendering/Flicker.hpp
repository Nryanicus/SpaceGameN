#ifndef flicker_hpp
#define flicker_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include "PostEffect.hpp"
#include "utilities/Vector.hpp"
#include "utilities/utilities.hpp"

class Flicker : public PostEffect
{
private:
    sf::Clock clock;
    float noise_proportion, smear_magnitude;
    int smear_direction; // in degrees
    float elasped_time, lurch;

    virtual void prepare_textures(sf::Vector2u size);

public:
    Flicker();
    virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output);
    float get_pixelation_factor();

    sf::RenderTexture working_texture;
    sf::RenderTexture working_texture2;
};

#endif