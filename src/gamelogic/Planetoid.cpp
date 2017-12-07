#include "Planetoid.hpp"

Planetoid::Planetoid(int size, Hex position)
: size(size), position(position)
{
    int num_sides = 6*size;
    double radius;
    if (size == 1) // asteroid/moon
        radius = 1.0;
    else if (size == 2) // asteroid/moon
        radius = 2.7;
    else if (size == 3) // moon
        radius = 4.4;
    else if (size == 4) // moon/exoplanet
        radius = 6.12;
    else if (size == 5) // moon/exoplanet
        radius = 7.8;
    else if (size == 6) // exoplanet
        radius = 9.54;
    else
    {
        std::cout << "Warning, planetoid ith size > 6" << std::endl;
        radius = (num_sides*SQRT3) / cos(pi/num_sides);
    }
    mass = 2*size + 2*(size-1);
    radius *= HEX_SIZE;
    int draw_sides = num_sides;
    if (num_sides > 64)
        draw_sides = 64;
    shape.setPointCount(draw_sides);
    for (int k=0; k<draw_sides; k++)
    {
        double theta = 2*pi/draw_sides*(k+0.5);
        double x = radius*cos(theta);
        double y = radius*sin(theta);
        shape.setPoint(k, sf::Vector2f(x, y));
    }
    shape.setFillColor(BG_COLOUR);
    shape.setOutlineColor(Grey);
    shape.setOutlineThickness(5);

    shape.setPosition(axial_to_pixel(position.q, position.r).to_sfml());
}

void Planetoid::draw(sf::RenderTarget* target)
{
    target->draw(shape);
}

int Planetoid::find_landing_location(Hex pos, Hex prev_pos)
{
    return 0;
}
