#include "Background.hpp"

Background::Background()
{
}

void Background::draw(sf::RenderTarget* target, sf::Font* font, double zoom)
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

    sf::Text text("", *font);

    for (unsigned int i=0; i<std::min(left_side.size(), right_side.size()); i++)
    {
        std::vector<Hex> row = left_side[i].all_hexes_between(right_side[i]);
        for (Hex h: row)
        {
            h.draw(target);
            text.setString(std::to_string(h.q)+", "+std::to_string(h.r));
            Vector v = axial_to_pixel(h.q, h.r);
            text.setPosition(v.x, v.y);
            text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
            // target->draw(text);
        }
    }
}