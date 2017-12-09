#include "Bloom.hpp"

Bloom::Bloom()
{
    brightness_shader.loadFromFile("../res/shaders/brightness.frag", sf::Shader::Fragment);
    blur_shader.loadFromFile("../res/shaders/blur.frag", sf::Shader::Fragment);
    downsample_shader.loadFromFile("../res/shaders/downsample.frag", sf::Shader::Fragment);
    add_shader.loadFromFile("../res/shaders/add.frag", sf::Shader::Fragment);
}

void Bloom::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
    prepare_textures(input.getSize());

    filter_bright(input, brightness_texture);
    
    downsample(brightness_texture, first_pass_textures[0]);
    blur_multipass(first_pass_textures);

    downsample(first_pass_textures[0], second_pass_textures[0]);
    blur_multipass(second_pass_textures);

    add(first_pass_textures[0], second_pass_textures[0], first_pass_textures[1]);
    first_pass_textures[1].display();
    add(input, first_pass_textures[1], output);
}

void Bloom::prepare_textures(sf::Vector2u size)
{
    if (brightness_texture.getSize() != size)
    {
        brightness_texture.create(size.x, size.y);
        brightness_texture.setSmooth(true);

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

void Bloom::filter_bright(const sf::RenderTexture& input, sf::RenderTexture& output)
{
    brightness_shader.setUniform("source", input.getTexture());
    apply_shader(brightness_shader, output);
    output.display();
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

void Bloom::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
    blur_shader.setUniform("source", input.getTexture());
    blur_shader.setUniform("offsetFactor", offsetFactor);
    apply_shader(blur_shader, output);
    output.display();
}

void Bloom::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
    downsample_shader.setUniform("source", input.getTexture());
    downsample_shader.setUniform("sourceSize", sf::Vector2f(input.getSize()));
    apply_shader(downsample_shader, output);
    output.display();
}

void Bloom::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
{
    add_shader.setUniform("source", source.getTexture());
    add_shader.setUniform("bloom", bloom.getTexture());
    apply_shader(add_shader, output);
}

void Bloom::apply_shader(const sf::Shader& shader, sf::RenderTarget& output)
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
