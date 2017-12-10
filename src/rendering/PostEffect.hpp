#ifndef posteffect_hpp
#define posteffect_hpp

#include <SFML/Graphics.hpp>
#include "utilities/utilities.hpp"

class PostEffect
{
protected:
    sf::Shader brightness_shader, 
               blur_shader,
               downsample_shader,
               add_shader,
               noise_shader,
               smear_shader,
               abberation_shader;

    virtual void prepare_textures(sf::Vector2u size)=0;

    void smear(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f smear_vector);
    void aberrate(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f r_off, sf::Vector2f g_off, sf::Vector2f b_off);
    void noise(const sf::RenderTexture& input, sf::RenderTexture& output, float noise_proportion);
    void add(const sf::RenderTexture& input, const sf::RenderTexture& bloom, sf::RenderTexture& output);
    void filter_bright(const sf::RenderTexture& input, sf::RenderTexture& output);
    void blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offset_factor);
    void downsample(const sf::RenderTexture& input, sf::RenderTexture& output);

    void apply_to_target(const sf::RenderTexture& input, sf::RenderTarget& target);
    
    static void apply_shader(const sf::Shader& shader, sf::RenderTarget& target);

    PostEffect();

public:
    virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output)=0;
};

#endif