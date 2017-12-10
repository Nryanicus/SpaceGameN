#include "Bloom.hpp"

Bloom::Bloom()
: PostEffect()
{
}

void Bloom::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
    prepare_textures(input.getSize());

    filter_bright(input, working_texture);
    
    downsample(working_texture, first_pass_textures[0]);
    blur_multipass(first_pass_textures);

    downsample(first_pass_textures[0], second_pass_textures[0]);
    blur_multipass(second_pass_textures);

    add(first_pass_textures[0], second_pass_textures[0], first_pass_textures[1]);
    add(input, first_pass_textures[1], working_texture);

    apply_to_target(working_texture, output);
}

void Bloom::prepare_textures(sf::Vector2u size)
{
    if (working_texture.getSize() != size)
    {
        working_texture.create(size.x, size.y);
        working_texture.setSmooth(true);

        first_pass_textures[0].create(size.x / 2, size.y / 2);
        first_pass_textures[0].setSmooth(true);
        first_pass_textures[1].create(size.x / 2, size.y / 2);
        first_pass_textures[1].setSmooth(true);

        second_pass_textures[0].create(size.x / 4, size.y / 4);
        second_pass_textures[0].setSmooth(true);
        second_pass_textures[1].create(size.x / 4, size.y / 4);
        second_pass_textures[1].setSmooth(true);
    }
}

void Bloom::blur_multipass(RenderTextureArray& renderTextures)
{
    sf::Vector2u textureSize = renderTextures[0].getSize();

    for (std::size_t count = 0; count < GAUSS_PASSES; ++count)
    {
        blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
        blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
    }
}