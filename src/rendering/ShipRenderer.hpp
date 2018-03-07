#ifndef shiprenderer_hpp
#define shiprenderer_hpp

#include <SFML/Graphics.hpp>
#include "Animated.hpp"
#include "vertices.hpp"
#include "gamelogic/ShipGameObject.hpp"
#include "utilities/Hex.hpp"

const sf::Color SHIP_COLOUR(50, 255, 50, 255);
const sf::Color PLUME_COLOUR(255, 0, 0, 255);

const sf::Color DASHED_SHIP_COLOUR(100, 255, 100, 150);
const sf::Color DASHED_PLUME_COLOUR(255, 50, 50, 150);

const sf::Color COMPUTING_COLOUR(128, 128, 128, 200);
const sf::Color PATH_FOUND_COLOUR(128, 255, 128, 200);
const sf::Color PATH_NOT_FOUND_COLOUR(255, 128, 128, 200);

const Vector TEXT_OFFSET(0, 32);

const double LANDED_SHIP_OFFSET = 17.37125;
const double PLUME_ANIMATE_TIME = 0.25;
const double BLINK_ANIMATE_TIME = 0.5;
const double TEXT_DISPLAY_TIME = 1.5;
enum class PathfindUIState {Awaiting, NeedVelocity};

class ShipRenderer : public Animated
{
private:
    // state
    ShipGameObject* ship;

    // rendering
    sf::VertexArray ship_array;
    sf::VertexArray dashed_ship_array;
    sf::VertexArray plume_array;
    sf::VertexArray dashed_plume_array;
    bool blink;
    double elapsed_time_blink;
    double elapsed_time_plume;
    double elapsed_time_path;
    double elapsed_time_text;

    // pathfind ui
    PathfindUIState pathfinding_ui_state;
    Hex goal_position;
    Hex goal_velocity;
    bool display_text;
    sf::Font font;
    sf::Text text;

    void update_plumes(double dt);

    // helpers
    Vector get_interpolated_pos();
    Vector get_current_pos_rot(double* rot, bool* draw_burn);
    Vector get_next_pos_rot(double* rot, bool* draw_burn);

    void set_display_text(std::string str, sf::Color col, bool keep_time_zero=false);

    sf::Transform get_next_transform(bool* burn);
    sf::Transform get_current_transform(bool* burn);
    sf::Transform get_interpolated_transform(bool* burn);

public:
    double PATH_ANIMATE_TIME = 2;
    ShipRenderer(ShipGameObject* ship);
    void draw(sf::RenderTarget* target, double dt, Hex mouse_hex, bool debug=false);
    void take_path_input(Hex h);
};

#endif