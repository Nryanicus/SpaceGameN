#ifndef shiprenderer_hpp
#define shiprenderer_hpp

#include <SFML/Graphics.hpp>
#include "Animated.hpp"
#include "vertices.hpp"
#include "gamelogic/ShipGameObject.hpp"
#include "utilities/Hex.hpp"

const sf::Color SHIP_COLOUR(50, 255, 50, 200);
const sf::Color PLUME_COLOUR(255, 100, 50, 200);

const sf::Color DASHED_SHIP_COLOUR(50, 255, 50, 100);
const sf::Color DASHED_PLUME_COLOUR(255, 100, 50, 100);

const sf::Color FUTURE_SHIP_COLOUR(0, 255, 0, 50);

const double LANDED_SHIP_OFFSET = 17.37125;
const double PLUME_ANIMATE_TIME = 0.75;

enum class PathfindUIState {Awaiting, NeedVelocity};

class ShipRenderer : public Animated
{
private:
    // state
    ShipGameObject* ship;

    // rendering
    sf::VertexArray ship_array;
    sf::VertexArray dashed_ship_array;
    sf::VertexArray future_ship_array;
    sf::VertexArray plume_array;
    sf::VertexArray dashed_plume_array;
    bool blink;
    double elapsed_time_blink;
    double elapsed_time_plume;

    // pathfind ui
    PathfindUIState pathfinding_state;
    Hex goal_position;
    Hex goal_velocity;

    void update_plumes(double dt);

    Vector get_current_pos_rot(double* rot, bool* draw_burn);
    Vector get_next_pos_rot(double* rot, bool* draw_burn);

public:
    ShipRenderer(ShipGameObject* ship);
    void draw(sf::RenderTarget* target, double dt, bool debug=false);
    void take_path_input(Hex h);
};

#endif