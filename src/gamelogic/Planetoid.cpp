#include "Planetoid.hpp"

Planetoid::Planetoid(int size, Hex position)
: gravity_wave_radius(0), 
  rotation(0),
  size(size), position(position)
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
    else if (size == 7) // atmo only
    {
        radius = 11.29;
        assert(false);
    }
    else // dissallowed
        assert(false);

    collision = spiral_hex_ring(Hex(0, 0), size-1);
    surface = hex_ring(Hex(0,0), size);

    radius -= 0.25;

    std::vector<Hex> additional_collisions = hex_ring(Hex(0,0), size);
    for (int i=0; i<6; i++)
        additional_collisions.erase(std::remove(additional_collisions.begin(), additional_collisions.end(), index_to_dirc(i)*size), additional_collisions.end());
    collision.insert(collision.end(), additional_collisions.begin(), additional_collisions.end());

    mass = 2*size + 2*(size-1);

    // set elevation variations
    for (int i=0; i<size*6; i++)
        radii.push_back(radius);

    // ensure there's not too big a gap between starting and end elevations
    do
    {
        radii[size*6-1] += 0.25 * random_int(-1, 1);
        for (int i=size*6-2; i>0; i--)
        {
            radii[i] = radii[i+1] + 0.25 * random_int(-1, 1);
            if (radii[i] > radius+0.25) radii[i] = radius+0.25;
            if (radii[i] < radius-0.25) radii[i] = radius-0.25;
        }
    } while(abs(radii[0]-radii[size*6-1]) > 0.25);

    outline.setPrimitiveType(sf::PrimitiveType::LineStrip);
    fill.setPrimitiveType(sf::PrimitiveType::TriangleFan);

    for (int i=0; i<num_sides; i++)
    {
        radius = radii[i]*HEX_SIZE;
        double theta = 2*pi/num_sides*(i+0.5);
        double x = radius*cos(theta);
        double y = radius*sin(theta);
        Vector p1(x,y);
        sf::Vertex v = sf::Vertex(sf::Vector2f(x, y));
        v.color = sf::Color(100,100,100);
        outline.append(v);
        v.color = BG_COLOUR;
        fill.append(v);

        theta = 2*pi/num_sides*(i+1+0.5);
        x = radius*cos(theta);
        y = radius*sin(theta);
        Vector p2(x,y);
        v = sf::Vertex(sf::Vector2f(x, y));
        v.color = sf::Color(100,100,100);
        outline.append(v);
        v.color = BG_COLOUR;
        fill.append(v);


        // cache landing positions
        Vector p = (p1+p2)/2;
        landing_locations.push_back(p);
        landing_angles.push_back(std::atan2(p.y, p.x)*RADIANS_TO_DEGREES); 
    }
    sf::Vertex v = outline[0];
    outline.append(v);

    // std::cout << size << " side length: " << Vector(outline.getPoint(0).x-outline.getPoint(1).x, outline.getPoint(0).y-outline.getPoint(1).y).magnitude() << std::endl;

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
                
                circle.setRadius(65);
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
        gravity_wave.setOutlineThickness(mass-gravity_wave_radius);
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

    sf::Transform trans;
    trans.translate(axial_to_pixel(position.q, position.r).to_sfml());
    trans.rotate(rotation*360/(size*6));
    target->draw(fill, sf::RenderStates(trans));
    target->draw(outline, sf::RenderStates(trans));
}

int Planetoid::find_landing_location(Hex pos, Hex prev_pos)
{
    if (pos.distance(prev_pos) > 1) return -1;
    // if the previous pos was on a surface hex we won't land by moving into
    // another (we're in orbit)
    if (size != 1)
        // situation one, we were in space now we're on a surface hex
        for (unsigned int i=0; i<surface.size(); i++)
            if ((surface[i] + position) == pos)
            {
                int landing_location = i-rotation-2;
                if (landing_location < 0) landing_location += size*6;
                std::cout << landing_location << " " << rotation << std::endl;
                return landing_location;
            }

    // situation two, we were previously on a surface hex
    // and have moved into the planet
    if (std::find(collision.begin(), collision.end(), pos-position) == collision.end())
        return -1;
    for (unsigned int i=0; i<surface.size(); i++)
        if ((surface[i] + position) == prev_pos)
        {
            int landing_location = i-rotation-2;
            if (landing_location < 0) landing_location += size*6;
            std::cout << landing_location << " " << rotation << std::endl;
            return landing_location;
        }

    return -1;
}

// get drawing parameters of a particular landing location
double Planetoid::get_landing_position_angle(int landing_location, Vector* p)
{
    sf::Transform trans;
    trans.translate(axial_to_pixel(position.q, position.r).to_sfml());
    trans.rotate(rotation*360.0/float(size*6));

    sf::Vector2f sfp = trans.transformPoint(landing_locations[landing_location].to_sfml());
    (*p) = Vector(sfp.x, sfp.y);
    return landing_angles[landing_location] + rotation*360.0/float(size*6); 
}

Hex Planetoid::get_landed_ship_position(int landing_location)
{
    return position+surface[(landing_location+rotation)%(size*6)];
}

void Planetoid::update()
{
    rotation = (rotation+1)%(size*6);
}