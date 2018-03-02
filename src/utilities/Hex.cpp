#include "Hex.hpp"

Hex Hex::from_pixels(double x, double y)
{
    std::pair<int, int> qr = pixel_to_axial(x, y);
    return Hex(qr.first, qr.second);
}

int Hex::distance(Hex other)
{
    return (abs(q - other.q)
          + abs(q + r - other.q - other.r) 
          + abs(r - other.r)) / 2;
}
int Hex::magnitude()
{
    int x = q;
    int z = r;
    int y = -x-z;
    return std::max(std::max(x, y), z);
}

Hex Hex::operator+(Hex o)
{
    return Hex(q+o.q, r+o.r);
}

Hex Hex::operator+=(Hex o)
{
    q += o.q;
    r += o.r;
    return *this;
}

Hex Hex::operator-(Hex o)
{
    return Hex(q-o.q, r-o.r);
}

Hex Hex::operator-=(Hex o)
{
    q -= o.q;
    r -= o.r;
    return *this;
}

Hex Hex::operator*(int m)
{
    return Hex(q*m, r*m);
}

Hex Hex::operator*=(int m)
{
    q *= m;
    r *= m;
    return *this;
}

bool Hex::operator!=(Hex o)
{
    return !(q == o.q && r == o.r);
}

void Hex::draw(sf::RenderTarget* target, bool fill, sf::Color col)
{
    // std::cout << "Warning! Hex.draw deprecated" << std::endl;

    sf::ConvexShape shape;
    shape.setPointCount(6);
    for (int k=0; k<6; k++)
    {
        double theta = 2*pi/6*(k+0.5);
        double x = HEX_SIZE*cos(theta);
        double y = HEX_SIZE*sin(theta);
        shape.setPoint(k, sf::Vector2f(x, y));
    }

    Vector v = axial_to_pixel(q, r);
    shape.setPosition(v.x, v.y);

    if (fill)
        shape.setFillColor(col);
    else
        shape.setFillColor(Transparent);
    shape.setOutlineColor(col);
    shape.setOutlineThickness(1);
    target->draw(shape);
}

/*
    Get all hexes in a line between this Hex and another
*/
std::vector<Hex> Hex::all_hexes_between(Hex o, bool alt)
{
    int d = distance(o);
    std::vector<Hex> results;
    
    // add 1e-6 to make line not land on boundaries, alt to land on opposite boundary
    double mod = 1e-6;
    if (alt) mod *= -1;

    double x1 = q + mod;
    double z1 = r + mod;
    double y1 = -x1-z1 - 2*mod;
    
    double x2 = o.q + mod;
    double z2 = o.r + mod;
    double y2 = -x2-z2 - 2*mod;
    for (int i=0; i<=d; i++)
    {
        double t = 1.0/d*i;
        std::pair<int, int> h = round_hex(lerp(x1, x2, t), lerp(y1, y2, t), lerp(z1, z2, t));
        results.push_back(Hex(h.first, h.second));
    }
    return results;
}

std::ostream& operator<<(std::ostream& os, const Hex& h)
{
    os << "H: " << h.q << ", " << h.r;
    return os;
}

Hex operator*(int m, Hex h)
{
    return h*m;
}

std::vector<Hex> spiral_hex_ring(Hex centre, int radius)
{
    assert(radius >= 0 && "invalid radius argument to spiral_hex_ring");
    std::vector<Hex> hexes;
    for (int i=0; i<=radius; i++)
    {
        std::vector<Hex> ring = hex_ring(centre, i);
        hexes.insert(hexes.end(), ring.begin(), ring.end());
    }
    return hexes;
}

std::vector<Hex> hex_ring(Hex centre, int radius)
{
    assert(radius >= 0 && "invalid radius argument to hex_ring");
    std::vector<Hex> hexes;
    if (radius == 0)
    {
        hexes.push_back(centre);
        return hexes;
    }

    // has the hex cardinal directions
    Hex h = Hex(1, 0) * radius;
    for (int i=0; i<6; i++)
        for (int j=0; j<radius; j++)
        {
            hexes.push_back(h);
            h += index_to_dirc(i+2); // +2 so we have a nice ring starting from rightmost hex
        }
    return hexes;
}

Hex index_to_dirc(int i)
{
    i=i%6;
    if (i == 0)
        return Hex(1, 0);
    if (i == 1)
        return Hex(0, 1);
    if (i == 2)
        return Hex(-1, 1);
    if (i == 3)
        return Hex(-1, 0);
    if (i == 4)
        return Hex(0, -1);
    if (i == 5)
        return Hex(1, -1);
    assert(false);
}

int dirc_to_index(Hex h)
{
    if (h == Hex(1, 0))
        return 0;
    if (h == Hex(0, 1))
        return 1;
    if (h == Hex(-1, 1))
        return 2;
    if (h == Hex(-1, 0))
        return 3;
    if (h == Hex(0, -1))
        return 4;
    if (h == Hex(1, -1))
        return 5;
    assert(false);
}

Hex rotate_right(Hex hex)
{
    if (hex == Hex(+1, 0))
        return Hex(0, 1);
    if (hex == Hex(0, 1))
        return Hex(-1, 1);
    if (hex == Hex(-1, 1))
        return Hex(-1, 0);
    if (hex == Hex(-1, 0))
        return Hex(0, -1);
    if (hex == Hex(0, -1))
        return Hex(1, -1);
    if (hex == Hex(1, -1))
        return Hex(1, 0);
    assert(false);
}

Hex rotate_left(Hex hex)
{
    if (hex == Hex(+1, 0))
        return Hex(1, -1);
    if (hex == Hex(0, 1))
        return Hex(1, 0);
    if (hex == Hex(-1, 1))
        return Hex(0, 1);
    if (hex == Hex(-1, 0))
        return Hex(-1, 1);
    if (hex == Hex(0, -1))
        return Hex(-1, 0);
    if (hex == Hex(1, -1))
        return Hex(0, -1);
    assert(false);
}

Hex rotate_hex(Hex hex, int dirc)
{
    if (dirc > 0)
    {
        while (dirc != 0)
        {
            hex = rotate_right(hex);
            dirc --;
        }
    }
    else
    {
        while (dirc != 0)
        {
            hex = rotate_left(hex);
            dirc ++;
        }
    }
    return hex;
}

sf::Vector2f hex_to_sfml_vec(Hex* h)
{
    Vector v = axial_to_pixel(h->q, h->r);
    return sf::Vector2f(v.x, v.y);
}

sf::Vector2f hex_to_sfml_vec(Hex h)
{
    Vector v = axial_to_pixel(h.q, h.r);
    return sf::Vector2f(v.x, v.y);
}

Hex sfml_vec_to_hex(sf::Vector2f v)
{
    std::pair<int, int> qr = pixel_to_axial(v.x, v.y);
    return Hex(qr.first, qr.second);
}
Hex sfml_vec_to_hex(sf::Vector2i v)
{
    return sfml_vec_to_hex(sf::Vector2f(v));
}

bool operator==(const Hex& l, const Hex& r)
{
    return l.r==r.r && l.q==r.q;
}