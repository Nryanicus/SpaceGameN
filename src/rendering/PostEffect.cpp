#include "PostEffect.hpp"

PostEffect::PostEffect()
{
    brightness_shader.loadFromFile("../res/shaders/brightness.frag", sf::Shader::Fragment);
    blur_shader.loadFromFile("../res/shaders/blur.frag", sf::Shader::Fragment);
    downsample_shader.loadFromFile("../res/shaders/downsample.frag", sf::Shader::Fragment);
    add_shader.loadFromFile("../res/shaders/add.frag", sf::Shader::Fragment);
    noise_shader.loadFromFile("../res/shaders/noise.frag", sf::Shader::Fragment);
    smear_shader.loadFromFile("../res/shaders/smear.frag", sf::Shader::Fragment);
    abberation_shader.loadFromFile("../res/shaders/chromatic_aberration.frag", sf::Shader::Fragment);
}

void PostEffect::smear(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f smear_vector)
{
    smear_shader.setUniform("source", input.getTexture());
    smear_shader.setUniform("direction", smear_vector);
    apply_shader(smear_shader, output);
    output.display();
}

void PostEffect::aberrate(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f r_off, sf::Vector2f g_off, sf::Vector2f b_off)
{
    abberation_shader.setUniform("source", input.getTexture());
    abberation_shader.setUniform("bOffset", b_off);
    abberation_shader.setUniform("rOffset", r_off);
    abberation_shader.setUniform("gOffset", g_off);
    apply_shader(abberation_shader, output);
    output.display();
}

void PostEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTexture& output)
{
    add_shader.setUniform("source", source.getTexture());
    add_shader.setUniform("bloom", bloom.getTexture());
    apply_shader(add_shader, output);
    output.display();
}

void PostEffect::noise(const sf::RenderTexture& input, sf::RenderTexture& output, float noise_proportion)
{
    noise_shader.setUniform("source", input.getTexture());
    noise_shader.setUniform("rand", ((float)random_int(0,100000))/100000.0f);
    noise_shader.setUniform("noiseProportion", noise_proportion);
    apply_shader(noise_shader, output);
    output.display();
}

void PostEffect::filter_bright(const sf::RenderTexture& input, sf::RenderTexture& output)
{
    brightness_shader.setUniform("source", input.getTexture());
    apply_shader(brightness_shader, output);
    output.display();
}

void PostEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
    blur_shader.setUniform("source", input.getTexture());
    blur_shader.setUniform("offsetFactor", offsetFactor);
    apply_shader(blur_shader, output);
    output.display();
}

void PostEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
    downsample_shader.setUniform("source", input.getTexture());
    downsample_shader.setUniform("sourceSize", sf::Vector2f(input.getSize()));
    apply_shader(downsample_shader, output);
    output.display();
}

void PostEffect::apply_to_target(const sf::RenderTexture& input, sf::RenderTarget& target)
{
    add_shader.setUniform("source", input.getTexture());
    add_shader.setUniform("bloom", sf::Texture());
    apply_shader(add_shader, target);
}

void PostEffect::apply_shader(const sf::Shader& shader, sf::RenderTarget& target)
{
    sf::Vector2f outputSize = static_cast<sf::Vector2f>(target.getSize());

    sf::VertexArray vertices(sf::TrianglesStrip, 4);
    vertices[0] = sf::Vertex(sf::Vector2f(0, 0),            sf::Vector2f(0, 1));
    vertices[1] = sf::Vertex(sf::Vector2f(outputSize.x, 0), sf::Vector2f(1, 1));
    vertices[2] = sf::Vertex(sf::Vector2f(0, outputSize.y), sf::Vector2f(0, 0));
    vertices[3] = sf::Vertex(sf::Vector2f(outputSize),      sf::Vector2f(1, 0));

    sf::RenderStates states;
    states.shader    = &shader;
    states.blendMode = sf::BlendNone;

    target.draw(vertices, states);
}
