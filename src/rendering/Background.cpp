#include "Background.hpp"

Background::Background()
{
    hex_texture.loadFromFile("../res/sprites/hex.png");
    hex_texture.setSmooth(true);
    hex_sprite.setTexture(hex_texture);
    hex_sprite.setOrigin(hex_sprite.getLocalBounds().width/2, hex_sprite.getLocalBounds().height/2);
    hex_sprite.scale(0.51, 0.51);

    font.loadFromFile("../res/fonts/LiberationMono-Regular.ttf");
}

void Background::draw(sf::RenderTarget* target, double zoom, bool hex_nums)
{   
    if (zoom > 7) return;

    // draw hexes

    sf::Vector2f view_centre = target->getView().getCenter();
    double view_width = target->getView().getSize().x;
    double view_height = target->getView().getSize().y;
    Hex top_left  = sfml_vec_to_hex(view_centre + sf::Vector2f(-view_width/2, -view_height/2));
    Hex top_right = sfml_vec_to_hex(view_centre + sf::Vector2f(view_width/2, -view_height/2));
    Hex bot_left  = sfml_vec_to_hex(view_centre + sf::Vector2f(-view_width/2, view_height/2));
    Hex bot_right = sfml_vec_to_hex(view_centre + sf::Vector2f(view_width/2, view_height/2));
    
    // overscan by one hex
    top_left += Hex(0,-1);
    top_right += Hex(1,-1);
    bot_left += Hex(-1,1);
    bot_right += Hex(0,1);

    std::vector<Hex> left_side = top_left.all_hexes_between(bot_left);
    std::vector<Hex> right_side = top_right.all_hexes_between(bot_right);

    sf::Text text("", font);

    for (unsigned int i=0; i<std::min(left_side.size(), right_side.size()); i++)
    {
        std::vector<Hex> row = left_side[i].all_hexes_between(right_side[i]);
        for (Hex h: row)
        {
            Vector v = axial_to_pixel(h.q, h.r);
            hex_sprite.setPosition(v.to_sfml());
            sf::Color bg_col = BG_HEX_COLOUR;
            bg_col.a = (int) 50*(1-zoom/7);
            hex_sprite.setColor(bg_col);
            target->draw(hex_sprite);

            if (hex_nums)
            {
                text.setString(std::to_string(h.q)+", "+std::to_string(h.r));
                text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
                text.setPosition(v.to_sfml());
                target->draw(text);
            }
        }
    }
}