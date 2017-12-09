#include "Planetoid.hpp"

Planetoid::Planetoid(int size, Hex position)
: size(size), position(position)
{
    int num_sides = 6*size;
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
    else if (size == 7) // atmo only
    {
        radius = 11.29;
        assert(false);
    }
    else // dissallowed
        assert(false);

    collision = spiral_hex_ring(Hex(0, 0), size-1);
    surface = hex_ring(Hex(0,0), size);

    std::vector<Hex> additional_collisions = hex_ring(Hex(0,0), size);
    for (int i=0; i<6; i++)
        additional_collisions.erase(std::remove(additional_collisions.begin(), additional_collisions.end(), index_to_dirc(i)*size), additional_collisions.end());
    collision.insert(collision.end(), additional_collisions.begin(), additional_collisions.end());

    mass = 2*size + 2*(size-1);
    radius *= HEX_SIZE;
    shape.setPointCount(num_sides);
    for (int k=0; k<num_sides; k++)
    {
        double theta = 2*pi/num_sides*(k+0.5);
        double x = radius*cos(theta);
        double y = radius*sin(theta);
        shape.setPoint(k, sf::Vector2f(x, y));
    }
    shape.setFillColor(Transparent);
    shape.setOutlineColor(Grey);
    shape.setOutlineThickness(5);

    shape.setPosition(axial_to_pixel(position.q, position.r).to_sfml());
}

void Planetoid::draw(sf::RenderTarget* target, bool debug)
{
    if (debug)
    {
        for (Hex h: collision)
            (h+position).draw(target, true, sf::Color(255,0,0,50));
        for (Hex h: surface)
            (h+position).draw(target, false, Blue);
    }
    target->draw(shape);
}

int Planetoid::find_landing_location(Hex pos, Hex prev_pos, Hex* p)
{
    if (pos.distance(prev_pos) > 1) return -1;
    // if the previous pos was on a surface hex we won't land by moving into
    // another (we're in orbit)
    if (size != 1)
    {
        // situation one, we were in space now we're on a surface hex
        for (unsigned int i=0; i<surface.size(); i++)
            if ((surface[i] + position) == pos)
            {
                *p = pos;
                return i;
            }
    }

    // situation two, we were previously on a surface hex
    // and have moved into the planet
    if (std::find(collision.begin(), collision.end(), pos) == collision.end())
        return -1;
    for (unsigned int i=0; i<surface.size(); i++)
        if ((surface[i] + position) == prev_pos)
        {
            *p = prev_pos;
            return i;
        }

    return -1;
}

double Planetoid::get_landing_position_angle(int landing_location, Vector* p)
{
    double theta1;
    if (landing_location != 0)
        theta1 = 2*pi/(6*size)*(landing_location-1+0.5);
    else
        theta1 = 2*pi/(6*size)*(size*6-1+0.5);
    double theta2 = 2*pi/(6*size)*(landing_location+0.5);
    Vector p1 = radius*Vector(cos(theta1), sin(theta1));
    Vector p2 = radius*Vector(cos(theta2), sin(theta2));

    (*p) = (p1+p2)/2;
    double landing_angle = std::atan2(p->y, p->x)*RADIANS_TO_DEGREES; 
    (*p) += axial_to_pixel(position.q, position.r);

    return landing_angle;
}