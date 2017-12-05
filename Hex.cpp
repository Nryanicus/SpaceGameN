#include "Hex.hpp"

Hex Hex::from_pixels(float x, float y)
{
    std::pair<int, int> qr = pixel_to_axial(x, y);
    return Hex(qr.first, qr.second);
}

int Hex::distance(Hex other)
{
    return (abs(q - other.q)
          + abs(q + r - other.q - other.r) 
          + abs(r - other.r))/2;
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

void Hex::draw(sf::RenderTarget* target, bool fill, sf::Color col, int trans)
{
    assert (trans >= 0 && trans <= 255);
    // std::cout << "Warning! Hex.draw deprecated" << std::endl;

    sf::ConvexShape shape;
    shape.setPointCount(6);
    for (int k=0; k<6; k++)
    {
        float theta = 2*pi/6*(k+0.5);
        float x = cos(theta);
        float y = sin(theta);
        shape.setPoint(k, sf::Vector2f(x, y));
    }

    Vector v = axial_to_pixel(q, r);
    shape.setPosition(v.x, v.y);

    // do culling check
    sf::FloatRect bounds = shape.getGlobalBounds();
    sf::FloatRect viewport = sf::FloatRect(target->mapPixelToCoords(sf::Vector2i(0, 0)), sf::Vector2f(target->getSize()));
    if (! bounds.intersects(viewport))
        return;

    if (trans)
        col.a = trans;
    if (fill)
        shape.setFillColor(col);
    else
        shape.setFillColor(Transparent);
    shape.setOutlineColor(col);
    shape.setOutlineThickness(0.01);
    target->draw(shape);
}

/*
    Get all hexes in a line between this Hex and another
*/
std::vector<Hex> Hex::all_hexes_between(Hex a)
{
    int d = distance(a);
    std::vector<Hex> results;
    results.reserve(d);
    for (int i=0; i<d; i++)
    {
        float t = 1.0/d*i;

        // x = q, z = r
        // add 1e-6 to make line not land on boundaries
        std::pair<int, int> h = round_hex(q + (a.q + 0.0001 - q)*t, r + (a.r - 0.0002 - r)*t);
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

Hex index_to_dirc(int i)
{
    i++; i=i%6;
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
        while (dirc != 0)
        {
            hex = rotate_right(hex);
            dirc --;
        }
    else
        while (dirc != 0)
        {
            hex = rotate_left(hex);
            dirc ++;
        }
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