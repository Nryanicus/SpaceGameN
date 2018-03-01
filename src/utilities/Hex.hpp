#ifndef hex_hpp
#define hex_hpp

#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include "utilities.hpp"

class Hex
{
public:
    int q, r;

    Hex(std::pair<int, int> p) : q(p.first), r(p.second) {}

    Hex(int q=0, int r=0) : q(q), r(r) {}

    Hex(int x, int y, int z): Hex(x, z) {}

    static Hex from_pixels(double x, double y);
    int distance(Hex other);
    int magnitude();
    Hex operator+(Hex o);
    Hex operator+=(Hex o);
    Hex operator-(Hex o);
    Hex operator-=(Hex o);
    Hex operator*(int m);
    Hex operator*=(int m);
    bool operator!=(Hex o);
    void draw(sf::RenderTarget* target, bool fill=false, sf::Color col=sf::Color(255,255,255,50));
    std::vector<Hex> all_hexes_between(Hex a);
};

Hex operator*(int m, Hex h);

Hex index_to_dirc(int i);

int dirc_to_index(Hex h);

std::vector<Hex> spiral_hex_ring(Hex centre, int radius);
std::vector<Hex> hex_ring(Hex h, int d);

std::ostream& operator<<(std::ostream& os, const Hex& h);

Hex rotate_hex(Hex hex, int dirc);

sf::Vector2f hex_to_sfml_vec(Hex* h);
sf::Vector2f hex_to_sfml_vec(Hex h);

Hex sfml_vec_to_hex(sf::Vector2f v);
Hex sfml_vec_to_hex(sf::Vector2i v);

bool operator==(const Hex& l, const Hex& r);

// hash function
namespace std
{
    template<> struct hash<Hex>
    {
        typedef Hex argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& h) const
        {
            result_type const h1 ( std::hash<int>()(h.q) );
            result_type const h2 ( std::hash<int>()(h.r) );
            return h1 ^ (h2 << 1); 
            // h.q + h.r * N; N largest possible r or q good hash, but who knows N?
        }
    };
}

const std::vector<Hex> CARDINAL_DIRECTIONS = {Hex(1, 0), Hex(0, 1), Hex(-1, 1), Hex(-1, 0), Hex(0, -1), Hex(1, -1)};

#endif