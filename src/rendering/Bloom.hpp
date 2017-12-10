#ifndef bloom_hpp
#define bloom_hpp

#include <SFML/Graphics.hpp>
#include "PostEffect.hpp"
#include "utilities/constants.hpp"

class Bloom : public PostEffect
{
private:
    typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

    virtual void prepare_textures(sf::Vector2u size);

    void blur_multipass(RenderTextureArray& render_textures);

public:
    Bloom();
    virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output);

    sf::RenderTexture   working_texture;
    RenderTextureArray  first_pass_textures;
    RenderTextureArray  second_pass_textures;
};

#endif