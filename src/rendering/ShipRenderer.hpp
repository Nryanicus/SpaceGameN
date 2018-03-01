#ifndef shiprenderer_hpp
#define shiprenderer_hpp

#include <SFML/Graphics.hpp>
#include "vertices.hpp"
#include "gamelogic/ShipGameObject.hpp"
#include "utilities/Hex.hpp"

const sf::Color SHIP_COLOUR(50, 255, 50, 200);
const sf::Color DASHED_SHIP_COLOUR(50, 255, 50, 100);

const double LANDED_SHIP_OFFSET = 17.37125;

enum class PathfindUIState {Awaiting, NeedVelocity};

class ShipRenderer
{
private:
    // state
    ShipGameObject* ship;

    // rendering
    sf::VertexArray ship_array;
    sf::VertexArray dashed_ship_array;
    bool blink;
    double elapsed_time;
    // pathfind ui
    PathfindUIState pathfinding_state;
    Hex goal_position;

public:
    ShipRenderer(ShipGameObject* ship);
    void draw(sf::RenderTarget* target, double dt, bool debug=false);
    void take_path_input(Hex h);
};

#endif