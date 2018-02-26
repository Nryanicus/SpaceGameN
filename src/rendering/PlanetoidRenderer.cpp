#include "PlanetoidRenderer.hpp"

double get_radius(int size)
{
    if (size == 1) // asteroid/moon
        return 1.0;
    else if (size == 2) // asteroid/moon
        return 2.7;
    else if (size == 3) // moon
        return 4.4;
    else if (size == 4) // moon/exoplanet
        return 6.12;
    else if (size == 5) // moon/exoplanet
        return 7.8;
    else if (size == 6) // exoplanet
        return 9.54;
    else if (size == 7) // atmo only
        return 11.29;
    assert(false);
}

PlanetoidRenderer::PlanetoidRenderer(PlanetoidGameObject* planetoid)
: gravity_wave_radius(0), elapsed_time(0),
  planetoid(planetoid)
{
    double radius = get_radius(planetoid->size) - ELEVATION_QUANTUM;
    double atmo_radius = get_radius(planetoid->size+1);//+0.5;
    double ocean_radius = 0;

    if (planetoid->has_ocean)
        ocean_radius = HEX_SIZE*(radius + (planetoid->ocean_level)*ELEVATION_QUANTUM);

    // set elevation variations
    std::vector<double> radii;
    for (int i=0; i<planetoid->num_sides; i++)
        radii.push_back(radius + ELEVATION_QUANTUM*(planetoid->elevations[i]));

    outline.setPrimitiveType(sf::PrimitiveType::LineStrip);
    fill.setPrimitiveType(sf::PrimitiveType::Triangles);
    atmosphere.setPrimitiveType(sf::PrimitiveType::TriangleFan);

    sf::Vertex v;
    v.color = ATMOSPHERE_COLOUR;
    atmosphere.append(v);

    for (int i=0; i<planetoid->num_sides; i++)
    {
        // centre point, for fill
        v = sf::Vertex(sf::Vector2f(0, 0));
        v.color = BG_COLOUR;
        fill.append(v);

        // first point, for fill and outline
        radius = radii[i]*HEX_SIZE;
        double theta = 2*pi/planetoid->num_sides*(i+0.5);
        double x = radius*cos(theta);
        double y = radius*sin(theta);
        Vector p1(x,y);
        v = sf::Vertex(sf::Vector2f(x, y));
        v.color = sf::Color(100,100,100);
        outline.append(v);
        v.color = BG_COLOUR;
        fill.append(v);

        // second, for fill and outline
        theta = 2*pi/planetoid->num_sides*(i+1.5);
        x = radius*cos(theta);
        y = radius*sin(theta);
        Vector p2(x,y);
        v = sf::Vertex(sf::Vector2f(x, y));
        v.color = sf::Color(100,100,100);
        outline.append(v);
        v.color = BG_COLOUR;
        fill.append(v);

        // atmo
        theta = 2*pi/planetoid->num_sides*(i+0.5);
        x = atmo_radius*HEX_SIZE*cos(theta);
        y = atmo_radius*HEX_SIZE*sin(theta);
        v = sf::Vertex(sf::Vector2f(x, y));
        v.color = ATMOSPHERE_COLOUR;
        v.color.a = 30;
        atmosphere.append(v);

        // ocean
        if (planetoid->ocean_present[i])
        {
            theta = 2*pi/planetoid->num_sides*(i+0.5);
            x = ocean_radius*cos(theta);
            y = ocean_radius*sin(theta);
            Vector op1(x,y);
            theta = 2*pi/planetoid->num_sides*(i+1.5);
            x = ocean_radius*cos(theta);
            y = ocean_radius*sin(theta);
            Vector op2(x,y);

            // check previous and next sides for ocean to see if we're a coast
            bool start_coast = false;
            bool end_coast = false;
            if (i == 0)
                start_coast = !planetoid->ocean_present.back();
            else
                start_coast = !planetoid->ocean_present[i-1];
            if (i == planetoid->num_sides-1)
                end_coast = !planetoid->ocean_present.front();
            else
                end_coast = !planetoid->ocean_present[i+1];

            oceans.push_back(Ocean(op1, op2, start_coast, end_coast));
        }

        // cache landing positions
        Vector p = (p1+p2)/2;
        landing_locations.push_back(p);
        landing_angles.push_back(std::atan2(p.y, p.x)*RADIANS_TO_DEGREES); 
    }
    v = outline[0];
    outline.append(v);
    v = atmosphere[1];
    atmosphere.append(v);

    //////////////////////////////////////
    // gravity
    //////////////////////////////////////

    for(int i=planetoid->size; i < planetoid->mass; i++)
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
        sf::Color col = GRAVITY_COLOUR;
        col.a = 200 + 55.0*(1.0 - ((float(i)/(float)planetoid->mass)));

        gravity_shape.setOutlineColor(col);
        gravity_shape.setOutlineThickness(3*(planetoid->mass - i));

        gravity_shape.setPosition(axial_to_pixel(planetoid->position.q, planetoid->position.r).to_sfml());
        gravity_shapes.push_back(gravity_shape);
    }

    gravity_wave.setPointCount(6);
    gravity_wave_radius = planetoid->mass-1;
    for (int j=0; j<6; j++)
    {
        double theta = 2*pi/6*j;
        double x = SQRT3*HEX_SIZE*cos(theta);
        double y = SQRT3*HEX_SIZE*sin(theta);
        gravity_wave.setPoint(j, sf::Vector2f(x, y));
    }
    gravity_wave.setFillColor(Transparent);
    gravity_wave.setOutlineThickness(1);

    font.loadFromFile("../res/fonts/LiberationMono-Regular.ttf");

    text = sf::Text("", font);
    text.setFillColor(GRAVITY_WAVE_COLOUR);
    gravity_circle.setFillColor(BG_COLOUR);
    gravity_circle.setRadius(65);
    gravity_circle.setOrigin(gravity_circle.getLocalBounds().width/2, gravity_circle.getLocalBounds().height/2);

    // draw numbers
    for (int i=planetoid->size; i<planetoid->mass; i++)
        for (int j=0; j<6; j++)
        {
            double theta = 2*pi/6*j;
            double x = SQRT3*HEX_SIZE*i*cos(theta);
            double y = SQRT3*HEX_SIZE*i*sin(theta);

            gravity_positions.push_back(Vector(x, y));
        }

    //////////////////////////////////////
    // resources
    //////////////////////////////////////
    surface_tironium.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: SURFACE_TIRONIUM)
        surface_tironium.append(sf::Vertex(v.to_sfml(), TIRONIUM_COLOUR));
    subterranean_tironium.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: SUBTERRANEAN_TIRONIUM)
        subterranean_tironium.append(sf::Vertex(v.to_sfml(), TIRONIUM_COLOUR));

    surface_carbicon.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: SURFACE_CARBICON)
        surface_carbicon.append(sf::Vertex(v.to_sfml(), CARBICON_COLOUR));
    subterranean_carbicon.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: SUBTERRANEAN_CARBICON)
        subterranean_carbicon.append(sf::Vertex(v.to_sfml(), CARBICON_COLOUR));

    surface_helidrogen.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: SURFACE_HELIDROGEN)
        surface_helidrogen.append(sf::Vertex(v.to_sfml(), HELIDROGEN_COLOUR));

    subterranean_fundamentium.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: SUBTERRANEAN_FUNDAMENTIUM)
        subterranean_fundamentium.append(sf::Vertex(v.to_sfml(), FUNDAMENTIUM_COLOUR));
}

void PlanetoidRenderer::draw(sf::RenderTarget* target, double dt, bool gravity, bool debug)
{
    if (gravity)
    {
        // draw rings
        for (unsigned int i=0; i<gravity_shapes.size(); i++)
            target->draw(gravity_shapes[i]);

        for (unsigned int i=0; i< gravity_positions.size(); i++)
        {
            sf::Vector2f pos = (gravity_positions[i] + axial_to_pixel(planetoid->position.q, planetoid->position.r)).to_sfml();
            gravity_circle.setPosition(pos);
            target->draw(gravity_circle);

            text.setCharacterSize(32);
            text.setFillColor(GRAVITY_WAVE_COLOUR);
            text.setString(std::to_string(i/6+1));
            text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
            text.setPosition(pos);
            target->draw(text);
        }

        // draw gravity wave
        sf::Color col = GRAVITY_WAVE_COLOUR;
        col.a = 10 + 245.0*(1.0 - (gravity_wave_radius/(float)planetoid->mass));
        gravity_wave.setOutlineColor(col);
        gravity_wave.setScale(gravity_wave_radius, gravity_wave_radius);
        gravity_wave.setOutlineThickness(planetoid->mass-gravity_wave_radius);
        gravity_wave.setPosition(axial_to_pixel(planetoid->position.q, planetoid->position.r).to_sfml());

        target->draw(gravity_wave);

        double dt_mod = 1;

        if (planetoid->size == 1) dt_mod = 0.5;
        gravity_wave_radius -= dt*dt_mod * (planetoid->mass - gravity_wave_radius);

        if ((gravity_wave_radius < 0.5) ||
            (planetoid->size > 1 && gravity_wave_radius < planetoid->size))
             gravity_wave_radius = planetoid->mass-1;
    }

    // atmo, surface, collision hexes
    if (debug)
    {
        for (Hex h: planetoid->collision)
            (h+planetoid->position).draw(target, true, sf::Color(255,20,20,50));
        for (Hex h: planetoid->surface)
            (h+planetoid->position).draw(target, false, sf::Color(20,20,255,200));
        if (planetoid->has_atmosphere)
            for (Hex h: planetoid->atmosphere_collision)
                (h+planetoid->position).draw(target, false, sf::Color(20,255,20,200));
    }

    // atmo
    sf::Transform trans;
    trans.translate(axial_to_pixel(planetoid->position.q, planetoid->position.r).to_sfml());
    trans.rotate(planetoid->rotation*360/(planetoid->num_sides));
    if (planetoid->has_atmosphere)
        target->draw(atmosphere, sf::RenderStates(trans));

    // fill and outline
    target->draw(fill, sf::RenderStates(trans));
    target->draw(outline, sf::RenderStates(trans));

    // number
    text.setFillColor(White);
    text.setCharacterSize(32*planetoid->size);
    text.setString(planetoid->name);
    text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
    text.setPosition(axial_to_pixel(planetoid->position.q, planetoid->position.r).to_sfml());
    target->draw(text);

    // ocean
    elapsed_time += dt;
    if (elapsed_time > 2*pi) elapsed_time -= 2*pi;

    for (Ocean ocean: oceans)
        ocean.draw(target, trans, elapsed_time);

    // resources
    for (int i=0; i<planetoid->num_sides; i++)
    {
        Vector p;
        double rot = get_landing_position_angle(i, &p) - 90;
        sf::Vector2f surface_pos = p.to_sfml();
        get_landing_position_angle(i, &p, true);
        sf::Vector2f sub_pos = p.to_sfml();
        trans = sf::Transform();
        trans.translate(surface_pos);
        trans.rotate(rot);
        sf::Transform sub_trans;
        sub_trans.translate(sub_pos);
        sub_trans.rotate(rot);

        if (planetoid->surface_resources[i] & TIRONIUM)
            target->draw(surface_tironium, sf::RenderStates(trans));
        if (planetoid->surface_resources[i] & CARBICON)
            target->draw(surface_carbicon, sf::RenderStates(trans));
        if (planetoid->surface_resources[i] & HELIDROGEN)
            target->draw(surface_helidrogen, sf::RenderStates(trans));
        if (planetoid->subterranean_resources[i] & TIRONIUM)
            target->draw(subterranean_tironium, sf::RenderStates(sub_trans));
        if (planetoid->subterranean_resources[i] & CARBICON)
            target->draw(subterranean_carbicon, sf::RenderStates(sub_trans));
        if (planetoid->subterranean_resources[i] & FUNDAMENTIUM)
            target->draw(subterranean_fundamentium, sf::RenderStates(sub_trans));
    }
}

// get drawing parameters of a particular landing location
double PlanetoidRenderer::get_landing_position_angle(int landing_location, Vector* p, bool subterranean)
{
    sf::Transform trans;
    trans.translate(axial_to_pixel(planetoid->position.q, planetoid->position.r).to_sfml());
    trans.rotate(planetoid->rotation*360.0/float(planetoid->num_sides));

    sf::Vector2f sfp;
    if (subterranean)
        sfp = trans.transformPoint((landing_locations[landing_location] - landing_locations[landing_location].normalise()*30).to_sfml());
    else
        sfp = trans.transformPoint(landing_locations[landing_location].to_sfml());
    (*p) = Vector(sfp.x, sfp.y);
    return landing_angles[landing_location] + planetoid->rotation*360.0/float(planetoid->num_sides); 
}
