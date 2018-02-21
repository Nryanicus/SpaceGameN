#include "Ocean.hpp"

Ocean::Ocean(Vector start, Vector end, bool start_coast, bool end_coast)
: start(start), end(end), 
  start_coast(start_coast), end_coast(end_coast)
{
    vertex_array.setPrimitiveType(sf::PrimitiveType::LineStrip);
    vertex_array.resize(OCEAN_SAMPLE_FREQ+1);
}

void Ocean::update(double elapsed_time)
{
    for (double i=0; i<=OCEAN_SAMPLE_FREQ; i++)
    {
        double r = i/OCEAN_SAMPLE_FREQ;
        Vector p = start*r + end*(1-r);

        // remove sine wave as we approach land
        double s = 1;
        if (start_coast && r > 0.5)
            s = 1.25-r;
        if (end_coast && r < 0.5)
            s = 0.25+r;

        p += s*p*sin(3*elapsed_time + 2*pi*r)/100;

        vertex_array[(int)i] = sf::Vertex(sf::Vector2f(p.x, p.y), OCEAN_COLOUR);
    }
}

void Ocean::draw(sf::RenderTarget* target, sf::Transform trans, double elapsed_time)
{
    update(elapsed_time);
    target->draw(vertex_array, sf::RenderStates(trans));
}