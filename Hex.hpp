#ifndef hex_hpp
#define hex_hpp

#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include "utilities.hpp"

class Hex
{
private:
    static sf::Sprite;
public:
    int q, r;

    Hex(int q=0, int r=0) : q(q), r(r) {}

    Hex(int x, int y, int z): Hex(x, z) {}

    static Hex from_pixels(double x, double y);
    int distance(Hex other);
    Hex operator+(Hex o);
    Hex operator+=(Hex o);
    Hex operator-(Hex o);
    Hex operator-=(Hex o);
    Hex operator*(int m);
    Hex operator*=(int m);
    bool operator!=(Hex o);
    void draw(sf::RenderTarget* target, bool fill=false, sf::Color col=sf::Color(255,255,255,50), int trans=0);
    void static draw_textured(sf::RenderTarget* target);
    std::vector<Hex> all_hexes_between(Hex a);
};

Hex operator*(int m, Hex h);

Hex index_to_dirc(int i);

int dirc_to_index(Hex h);

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

#endif