#include "Background.hpp"

Background::Background()
{
    if (!tile_texture.loadFromFile("res/hex_tile_sym.png"))
        std::cout <<"Error loading background tile_texture" << std::endl;
    tile_texture.setSmooth(true);
    tile_texture.setRepeated(true);

    sf::Vector2u tex_size = tile_texture.getSize();

    tile.setTexture(tile_texture);
    tile.setTextureRect(sf::IntRect(0, 0, tex_size.x, tex_size.y));
    tile.setOrigin(((float)tex_size.x)/2.0, ((float)tex_size.y)/2.0);
    tile.setScale(2*HEX_WIDTH/((float)tex_size.x), 2*HEX_WIDTH/((float)tex_size.x));
    
    sf::FloatRect bounds = tile.getGlobalBounds();

    int num_cols = ((float)HEIGHT)/(8*bounds.height);
    int num_rows = ((float)WIDTH)/(8*bounds.width);
    
    tile.setTextureRect(sf::IntRect(0, 0, num_rows*tex_size.x, num_cols*tex_size.y));
    tile.setOrigin(num_rows*((float)tex_size.x)/2.0, num_cols*((float)tex_size.y)/2.0);
    tile.setPosition(0, 0);

    // init hexes array
    if (!single_hex_texture.loadFromFile("res/single_hex_middle.png"))
        std::cout <<"Error loading background single_hex_filled texture" << std::endl;
    single_hex_texture.setSmooth(true);
    
    // figure out how many hexes we need
    tex_size = single_hex_texture.getSize();
    int hexes_width = ((float) HEIGHT)/5.0f; // 1 being the starting zoom in ViewOpts
    int hexes_height = ((float) WIDTH)/5.0f;

    hexes.setPrimitiveType(sf::Quads);

    Hex top_left = sfml_vec_to_hex(sf::Vector2f(0, 0));
    Hex top_right = sfml_vec_to_hex(sf::Vector2f(hexes_width, 0));
    Hex bottom_left = sfml_vec_to_hex(sf::Vector2f(0, hexes_height));

    int d = abs(top_right.q - top_left.q);
    int k = 0;

    std::vector<Hex> left = bottom_left.all_hexes_between(top_left);

    hexes.resize(d*left.size()*4);
    for (int i=0; i<left.size(); i++)
    {
        Hex h = left[i];
        for (int j=0; j<d; j++)
        {
            assert(k*4 < hexes.getVertexCount());
            sf::Vertex* quad = &hexes[k * 4];
            Hex hex = Hex(h.q+j, h.r);
            sf::Vector2f pos = hex_to_sfml_vec(&hex);
            quad[0].position = pos + sf::Vector2f(-HEX_WIDTH/2, -HEX_HEIGHT/2);
            quad[1].position = pos + sf::Vector2f(+HEX_WIDTH/2, -HEX_HEIGHT/2);
            quad[2].position = pos + sf::Vector2f(+HEX_WIDTH/2, +HEX_HEIGHT/2);
            quad[3].position = pos + sf::Vector2f(-HEX_WIDTH/2, +HEX_HEIGHT/2);

            // define its 4 texture coordinates, zero'd out at start
            quad[0].texCoords = sf::Vector2f(0, 0);
            quad[1].texCoords = sf::Vector2f(0, 0);
            quad[2].texCoords = sf::Vector2f(0, 0);
            quad[3].texCoords = sf::Vector2f(0, 0);

            hex_to_vertex[hex] = quad;
            k++;
        }
    }
}

void Background::paint_hex(Hex to_paint, sf::Color colour)
{
    if (!hex_to_vertex.count(to_paint))
        return;        

    sf::Vertex* quad = hex_to_vertex[to_paint];
    sf::Vector2u tex_size = single_hex_texture.getSize();
    quad[0].texCoords = sf::Vector2f(0, 0);
    quad[1].texCoords = sf::Vector2f(tex_size.x, 0);
    quad[2].texCoords = sf::Vector2f(tex_size.x, tex_size.y);
    quad[3].texCoords = sf::Vector2f(0, tex_size.y);
    for (int j=0; j<4; j++)
        quad[j].color = colour;
}

void Background::paint_hexes(std::vector<Hex> to_paint, sf::Color col)
{
    for (int i = 0; i < to_paint.size(); ++i)
        paint_hex(to_paint[i], col);
}

void Background::clear_hexes(std::vector<Hex> to_clear)
{
    // for (int i = 0; i < to_clear.size(); ++i)
    // {
    //     // axial to cube
    //     int x = to_clear[i].q;
    //     int z = to_clear[i].r;
    //     int y = -x-z;
    //     // cube to odd-q
    //     int col = x;
    //     int row = z + (x * (x&1)) / 2;
    //     // set the appropriate verts' texture coords
    //     assert((row + col * hexes_width) * 4 < hexes.getVertexCount());
    //     sf::Vertex* quad = &hexes[(row + col * hexes_width) * 4];
    //     quad[0].texCoords = sf::Vector2f(0, 0);
    //     quad[1].texCoords = sf::Vector2f(0, 0);
    //     quad[2].texCoords = sf::Vector2f(0, 0);
    //     quad[3].texCoords = sf::Vector2f(0, 0);
    // }
}

void Background::clear_all_hexes()
{
    for (int i = 0; i < hexes.getVertexCount(); ++i)
        hexes[i].texCoords = sf::Vector2f(0, 0);
}



void Background::draw(sf::RenderTarget* target, sf::Font* font, float zoom)
{   
    // if (zoom >= 1)
    //     return;

    // translate tile so its consistent with axial coords
    sf::Vector2f view_centre = target->getView().getCenter();
    Hex centre_hex = Hex::from_pixels(view_centre.x, view_centre.y);
    Vector centre_xy = axial_to_pixel(centre_hex.q, centre_hex.r);
    tile.setPosition(centre_xy.x, centre_xy.y);

    sf::Color bg_col = BG_COLOUR;
    bg_col.a = (int) 255*(1-zoom);
    tile.setColor(bg_col);
    target->draw(tile);

    // draw hexes vertex array
    sf::RenderStates render_state;
    sf::Transform trans;

    // sf::Vector2f top_left = target->getView().getCenter() - target->getView().getSize()/2.0f;
    // Hex top_left_hex = Hex::from_pixels(top_left.x, top_left.y);
    Hex top_left_hex = sfml_vec_to_hex(target->getView().getCenter() - target->getView().getSize()/2.0f);
    Vector top_left_xy = axial_to_pixel(top_left_hex.q, top_left_hex.r);

    trans = trans.translate(top_left_xy.x, top_left_xy.y);
    render_state.transform = trans;
    render_state.texture = &single_hex_texture;
    target->draw(hexes, render_state);
}