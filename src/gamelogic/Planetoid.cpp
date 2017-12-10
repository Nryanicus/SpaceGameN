#include "Planetoid.hpp"

Planetoid::Planetoid(int size, Hex position)
: gravity_wave_radius(0), 
  size(size), position(position)
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
    for (int i=0; i<num_sides; i++)
    {
        double theta = 2*pi/num_sides*(i+0.5);
        double x = radius*cos(theta);
        double y = radius*sin(theta);
        shape.setPoint(i, sf::Vector2f(x, y));
    }
    shape.setFillColor(Transparent);
    shape.setOutlineColor(sf::Color(100,100,100));
    shape.setOutlineThickness(10);

    shape.setPosition(axial_to_pixel(position.q, position.r).to_sfml());

    //////////////////////////////////////
    // gravity
    //////////////////////////////////////

    for(int i=size; i < mass; i++)
    {
        sf::ConvexShape gravity_shape;
        gravity_shape.setPointCount(6);
        for (int j=0; j<6; j++)
        {
            double theta = 2*pi/6*j;
            double x = SQRT3*HEX_SIZE*i*cos(theta);
            double y = SQRT3*HEX_SIZE*i*sin(theta);
            gravity_shape.setPoint(j, sf::Vector2f(x, y));
        }
        gravity_shape.setFillColor(Transparent);
        sf::Color col = gravity_colour;
        col.a = 200 + 55.0*(1.0 - ((float(i)/(float)mass)));

        gravity_shape.setOutlineColor(col);
        gravity_shape.setOutlineThickness(3*(mass - i));

        gravity_shape.setPosition(axial_to_pixel(position.q, position.r).to_sfml());
        gravity_shapes.push_back(gravity_shape);
    }

    gravity_wave.setPointCount(6);
    gravity_wave_radius = mass-1;
    for (int j=0; j<6; j++)
    {
        double theta = 2*pi/6*j;
        double x = SQRT3*HEX_SIZE*cos(theta);
        double y = SQRT3*HEX_SIZE*sin(theta);
        gravity_wave.setPoint(j, sf::Vector2f(x, y));
    }
    gravity_wave.setFillColor(Transparent);
    gravity_wave.setOutlineThickness(1);

    gravity_wave.setPosition(axial_to_pixel(position.q, position.r).to_sfml());

    font.loadFromFile("../res/fonts/LiberationMono-Regular.ttf");
}

void Planetoid::draw(sf::RenderTarget* target, double dt, bool gravity, bool debug)
{
    if (gravity)
    {
        // draw rings
        for (unsigned int i=0; i<gravity_shapes.size(); i++)
            target->draw(gravity_shapes[i]);

        sf::Text text("", font);
        text.setFillColor(gravity_wave_colour);
        sf::CircleShape circle;
        circle.setFillColor(BG_COLOUR);
        // draw numbers
        for (int i=size; i<mass; i++)
            for (int j=0; j<6; j++)
            {
                double theta = 2*pi/6*j;
                double x = SQRT3*HEX_SIZE*i*cos(theta);
                double y = SQRT3*HEX_SIZE*i*sin(theta);

                Vector v = Vector(x, y) + axial_to_pixel(position.q, position.r);
                text.setString(std::to_string(mass - i));
                text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
                text.setPosition(v.to_sfml());
                
                circle.setRadius(50);
                circle.setOrigin(circle.getLocalBounds().width/2, circle.getLocalBounds().height/2);
                circle.setPosition(v.to_sfml());
                target->draw(circle);
                target->draw(text);
            }

        // draw gravity wave
        sf::Color col = gravity_wave_colour;
        col.a = 10 + 245.0*(1.0 - (gravity_wave_radius/(float)mass));
        gravity_wave.setOutlineColor(col);
        gravity_wave.setScale(gravity_wave_radius, gravity_wave_radius);
        gravity_wave.setOutlineThickness(5*mass/gravity_wave_radius);
        target->draw(gravity_wave);

        if (size == 1) dt *= 0.5;
        gravity_wave_radius -= dt * (mass - gravity_wave_radius);

        if ((gravity_wave_radius < 0.5) ||
            (size > 1 && gravity_wave_radius < size))
             gravity_wave_radius = mass-1;
    }

    if (debug)
    {
        for (Hex h: collision)
            (h+position).draw(target, true, sf::Color(255,20,20,50));
        for (Hex h: surface)
            (h+position).draw(target, false, sf::Color(20,20,255,200));
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