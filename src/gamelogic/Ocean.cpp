#include "Ocean.hpp"

Ocean::Ocean(Vector start, Vector end, double base_offset)
: start(start), end(end), 
  base_offset(base_offset)
{
    vertex_array.setPrimitiveType(sf::PrimitiveType::LineStrip);
    vertex_array.resize(OCEAN_SAMPLE_FREQ+1);
    update(0);
}

void Ocean::update(double elapsed_time)
{
    for (double i=0; i<=OCEAN_SAMPLE_FREQ; i++)
    {
        double r = i/OCEAN_SAMPLE_FREQ;
        Vector p = start*r + end*(1-r);

        p += p*sin(3*elapsed_time + 2*pi*r)/100;

        vertex_array[(int)i] = sf::Vertex(sf::Vector2f(p.x, p.y), OCEAN_COLOUR);
    }
}

void Ocean::draw(sf::RenderTarget* target, sf::Transform trans, double elapsed_time)
{
    update(elapsed_time);
    target->draw(vertex_array, sf::RenderStates(trans));
}