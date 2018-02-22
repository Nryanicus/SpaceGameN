#include "PlanetoidGameObject.hpp"

PlanetoidGameObject::PlanetoidGameObject(int size, Hex position)
: rotation(0), num_sides(6*size), size(size), mass(2*size + 2*(size-1)),
  has_atmosphere(false), has_ocean(false),
  position(position)
{
    // only larger planets can have atmos and oceans
    if (size > 1)
    {
        has_atmosphere = random_int(0, 1);
        if (has_atmosphere)
            has_ocean = random_int(0, 1);
        if (has_ocean)
            ocean_level = random_int(1, 2);
    }

    // determine which hexes we're occupying
    collision = spiral_hex_ring(Hex(0, 0), size-1);
    surface = hex_ring(Hex(0,0), size);
    atmosphere_collision = hex_ring(Hex(0,0), size+1);

    std::vector<Hex> additional_collisions = hex_ring(Hex(0,0), size);
    for (int i=0; i<6; i++)
    {
        Hex h = index_to_dirc(i)*size;
        additional_collisions.erase(std::remove(additional_collisions.begin(), additional_collisions.end(), h), additional_collisions.end());
        atmosphere_collision.push_back(h);
    }
    collision.insert(collision.end(), additional_collisions.begin(), additional_collisions.end());

    if (!has_atmosphere)
        atmosphere_collision.clear();

    // set elevation variations
    for (int i=0; i<num_sides; i++)
        elevations.push_back(1);

    // randomise elevations, ensure the diff between start and end isn't too great
    do
    {
        elevations[num_sides-1] += random_int(-1, 1);
        for (int i=num_sides-2; i>0; i--)
        {
            elevations[i] = elevations[i+1] + random_int(-1, 1);
            if (elevations[i] > 2) elevations[i] = 2;
            if (elevations[i] < 0) elevations[i] = 0;
        }
    } while(abs(elevations[0]-elevations[num_sides-1]) > 1);

    // determine where oceans are
    for (int i=0; i<num_sides; i++)
        ocean_present.push_back(has_ocean && (elevations[i] < ocean_level));

    // determine what resources we have
    for (int i=0; i<num_sides; i++)
    {
        surface_resources.push_back(0);
        subterranean_resources.push_back(0);
    }
    // surface tironium
    if (!random_int(0, 3))
        for (int i=0; i<random_int(0, num_sides/2); i++)
        {
            int j = random_int(0, num_sides);
            surface_resources[j] = surface_resources[j] | TIRONIUM;
        }
    // surface carbicon
    if (!random_int(0, 3))
        for (int i=0; i<random_int(0, num_sides/2); i++)
        {
            int j = random_int(0, num_sides);
            surface_resources[j] = surface_resources[j] | CARBICON;
        }
    // surface solid helidrogen
    if (!has_atmosphere && !random_int(0, 3))
        for (int i=0; i<random_int(0, num_sides/2); i++)
        {
            int j = random_int(0, num_sides);
            surface_resources[j] = surface_resources[j] | HELIDROGEN;
        }

    // subterrean tironium
    for (int i=0; i<num_sides; i++)
        if (surface_resources[i] & TIRONIUM && !random_int(0, 2))
            subterranean_resources[i] = subterranean_resources[i] | TIRONIUM;
    // subterrean carbicon
    for (int i=0; i<num_sides; i++)
        if (surface_resources[i] & CARBICON && !random_int(0, 2))
            subterranean_resources[i] = subterranean_resources[i] | CARBICON;
    // subterrean fundamentium
    // if (!random_int(0, 6))
        for (int i=0; i<num_sides; i++)
            if (!surface_resources[i] && !random_int(0, 10))
                subterranean_resources[i] = FUNDAMENTIUM;

    std::cout << "planet " << size << " surface resources:" << std::endl;
    for (int i=0; i<num_sides; i++)
        std::cout << surface_resources[i] << " ";
    std::cout << std::endl;
    std::cout << "planet " << size << " subterrean resources:" << std::endl;
    for (int i=0; i<num_sides; i++)
        std::cout << subterranean_resources[i] << " ";
    std::cout << std::endl;
}

int PlanetoidGameObject::find_landing_location(Hex pos, Hex prev_pos)
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
                if (landing_location < 0) landing_location += num_sides;
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
            if (landing_location < 0) landing_location += num_sides;
            std::cout << landing_location << " " << rotation << std::endl;
            return landing_location;
        }

    return -1;
}

Hex PlanetoidGameObject::get_landed_ship_position(int landing_location)
{
    return position+surface[(landing_location+rotation+2)%(num_sides)];
}

void PlanetoidGameObject::update()
{
    rotation = (rotation+1)%(num_sides);
}