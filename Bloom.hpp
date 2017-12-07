#ifndef bloom_hpp
#define bloom_hpp

#include <SFML/Graphics.hpp>

class Bloom
{
private:
    typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

    void prepare_textures(sf::Vector2u size);

    void filter_bright(const sf::RenderTexture& input, sf::RenderTexture& output);
    void blur_multipass(RenderTextureArray& render_textures);
    void blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offset_factor);
    void downsample(const sf::RenderTexture& input, sf::RenderTexture& output);
    void add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);

    static void apply_shader(const sf::Shader& shader, sf::RenderTarget& output);


public:
    Bloom();
    void apply(const sf::RenderTexture& input, sf::RenderTarget& output);

    sf::Shader brightness_shader, blur_shader, downsample_shader, add_shader;

    sf::RenderTexture   brightness_texture;
    RenderTextureArray  first_pass_textures;
    RenderTextureArray  second_pass_textures;
};

#endif