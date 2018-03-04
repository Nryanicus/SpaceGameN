#include "ShipRenderer.hpp"

ShipRenderer::ShipRenderer(ShipGameObject* ship)
: Animated(),
  ship(ship),
  blink(false), elapsed_time_blink(0),
  elapsed_time_plume(0), elapsed_time_path(0), elapsed_time_text(0),
  pathfinding_ui_state(PathfindUIState::Awaiting), display_text(false)
{
    ship_array.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: MILITARY_SHIP)
        ship_array.append(sf::Vertex(v.to_sfml(), SHIP_COLOUR));

    dashed_ship_array.setPrimitiveType(sf::PrimitiveType::Lines);
    for (Vector v: DASHED_MILITARY_SHIP)
        dashed_ship_array.append(sf::Vertex(v.to_sfml(), DASHED_SHIP_COLOUR));

    plume_array.setPrimitiveType(sf::PrimitiveType::Lines);
    plume_array.resize(8);

    dashed_plume_array.setPrimitiveType(sf::PrimitiveType::Lines);
    dashed_plume_array.resize(8);

    font.loadFromFile("../res/fonts/LiberationMono-Regular.ttf");
    text = sf::Text("", font);
}

Vector ShipRenderer::get_next_pos_rot(double* rot, bool* draw_burn)
{
    // show planned burn
    *rot = 60*ship->rotation;
    *draw_burn = false;
    if (ship->planned_accelerations.size() != 0)
        if (ship->planned_accelerations[0] != Hex())
        {
            std::vector<Hex> acceleration_line = ship->planned_accelerations[0].all_hexes_between(Hex());
            int s = acceleration_line.size();
            assert(s >= 2);
            int next_rotation = dirc_to_index(acceleration_line[s-2]);
            *rot = 60*next_rotation;
            *draw_burn = true;
        }

    return axial_to_pixel(ship->position.q+ship->velocity.q, ship->position.r+ship->velocity.r);
}

Vector ShipRenderer::get_current_pos_rot(double* rot, bool* draw_burn)
{
    Vector pos;
    *draw_burn = false;
    if (ship->landed)
        *rot = ship->landed_planetoid->get_landing_position_angle(ship->landed_location, &pos, LANDED_SHIP_OFFSET);
    else
    {
        *draw_burn = ship->acceleration != Hex();
        *rot = 60*ship->rotation;
        pos = axial_to_pixel(ship->position.q, ship->position.r);
    }
    return pos;
}

void ShipRenderer::draw(sf::RenderTarget* target, double dt, Hex mouse_hex, bool debug)
{
    // if ship has received input ensure the preview is visible
    if (ship->reset_position_preview)
    {
        blink = false;
        elapsed_time_blink = 0;
        ship->reset_position_preview = false;
    }

    double rot;
    bool draw_burn;
    Vector pos = get_current_pos_rot(&rot, &draw_burn);
    double current_rot = rot;
    // interpolate if we're animating
    if (animation_state != AnimationState::None && !ship->landed)
    {
        double rot2;
        bool draw_burn2;
        Vector pos2 = get_next_pos_rot(&rot2, &draw_burn2);
        double r = elapsed_time_animate/TOTAL_TIME_ANIMATE;
        pos = (1-r)*pos + r*pos2;

        if (abs(rot2-rot) > 180)
        {
            if (rot2 > rot)
                rot += 360;
            else
                rot2 += 360;
        }
        rot = rot + (rot2-rot)*r;
    }

    sf::Transform trans;
    trans.translate(pos.to_sfml());
    trans.rotate(rot);
    target->draw(ship_array, sf::RenderStates(trans));
    if (draw_burn)
        target->draw(plume_array, sf::RenderStates(trans));

    // draw pathfinding UI
    if (pathfinding_ui_state == PathfindUIState::NeedVelocity)
    {
        double bounce = 0.9 - 0.5*sin(elapsed_time_blink/BLINK_ANIMATE_TIME*pi);

        trans = sf::Transform();
        Vector goal_pos = axial_to_pixel(goal_position.q, goal_position.r);
        trans.translate(goal_pos.to_sfml());
        trans.rotate(current_rot);
        trans.scale(bounce, bounce);
        target->draw(ship_array, sf::RenderStates(trans));

        trans = sf::Transform();
        Vector mouse_pos = axial_to_pixel(mouse_hex.q, mouse_hex.r);
        trans.translate(mouse_pos.to_sfml());
        trans.rotate(current_rot);
        trans.scale(bounce, bounce);
        target->draw(dashed_ship_array, sf::RenderStates(trans));
    }

    // draw planned trajectory
    if (ship->planned_accelerations.size() > 0)
    {
        Hex next_pos = ship->position;
        Hex next_vel = ship->velocity;
        int next_rot = ship->rotation;

        std::vector<Hex> future_pos;
        std::vector<int> future_rot;
        std::vector<bool> future_accel;

        for (Hex a : ship->planned_accelerations)
        {
            next_pos += next_vel;
            next_vel += a;
            for (Planetoid* p: *ship->planets)
                next_vel += p->get_gravity_at_point(next_pos);

            if (a != Hex())
            {
                std::vector<Hex> acceleration_line = a.all_hexes_between(Hex());
                int s = acceleration_line.size();
                assert(s >= 2);
                next_rot = dirc_to_index(acceleration_line[s-2]);
            }

            future_pos.push_back(next_pos);
            future_rot.push_back(next_rot);
            future_accel.push_back(a != Hex());
        }

        int draw_index = ceil( ((double) future_pos.size())*elapsed_time_path/PATH_ANIMATE_TIME ) -1;
        draw_index = draw_index<0 ? 0 : draw_index;
        trans = sf::Transform();
        pos = axial_to_pixel(future_pos[draw_index].q, future_pos[draw_index].r);
        trans.translate(pos.to_sfml());
        trans.rotate(future_rot[draw_index]*60);
        target->draw(dashed_ship_array, sf::RenderStates(trans));
        if (future_accel[draw_index])
            target->draw(dashed_plume_array, sf::RenderStates(trans));

    }
    // draw next position
    else if (!blink && (ship->velocity != Hex() || ship->taking_off || ship->acceleration != Hex()))
    {
        if (debug)
            for (Hex movement_hex: (ship->position+ship->velocity).all_hexes_between(ship->position))
                movement_hex.draw(target, true, sf::Color(50, 255, 50, 100));
        
        pos = get_next_pos_rot(&rot, &draw_burn);

        trans = sf::Transform();
        trans.translate(pos.to_sfml());
        trans.rotate(rot);
        double bounce = 1.25 - 0.25*sin(elapsed_time_blink/BLINK_ANIMATE_TIME*pi);
        trans.scale(bounce, bounce);
        target->draw(dashed_ship_array, sf::RenderStates(trans));
        if (draw_burn)
            target->draw(dashed_plume_array, sf::RenderStates(trans));
    }

    if (ship->pathfinding_state == PathfindingState::PathNotFound)
    {
        ship->pathfinding_state = PathfindingState::None;
        set_display_text("Path Not Found", PATH_NOT_FOUND_COLOUR);
    }
    if (ship->pathfinding_state == PathfindingState::PathFound)
    {
        ship->pathfinding_state = PathfindingState::None;
        set_display_text("Path Found", PATH_FOUND_COLOUR);
    }
    if (ship->pathfinding_state == PathfindingState::Computing)
        set_display_text("Computing", COMPUTING_COLOUR, true);
    if (display_text)
    {
        target->draw(text);
        elapsed_time_text += dt;
        if (elapsed_time_text > TEXT_DISPLAY_TIME)
        {
            display_text = false;
            elapsed_time_text = 0;
        }
    }

    elapsed_time_blink += dt;
    if (elapsed_time_blink > BLINK_ANIMATE_TIME)
    {
        elapsed_time_blink = 0;
        blink = ! blink;
    }
    elapsed_time_path += dt;
    if (elapsed_time_path > PATH_ANIMATE_TIME)
        elapsed_time_path = 0;


    update_plumes(dt);
}

void ShipRenderer::set_display_text(std::string str, sf::Color col, bool keep_time_zero)
{
    text.setFillColor(col);

    text.setCharacterSize(32);
    text.setString(str);
    text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
    Vector pos = axial_to_pixel(ship->position.q, ship->position.r) + TEXT_OFFSET;
    text.setPosition(pos.to_sfml());

    if (keep_time_zero)
        elapsed_time_text = 0;
    display_text = true;
}

void ShipRenderer::update_plumes(double dt)
{
    elapsed_time_plume += dt;
    if (elapsed_time_plume > PLUME_ANIMATE_TIME)
        elapsed_time_plume = 0;
    double r = elapsed_time_plume/PLUME_ANIMATE_TIME;
    Vector v1 = MILITARY_SHIP_PLUME[0][0];
    Vector v2 = MILITARY_SHIP_PLUME[1][0];
    plume_array[0] = sf::Vertex((v1*r + v2*(1-r)).to_sfml(), PLUME_COLOUR);
    v1 = MILITARY_SHIP_PLUME[0][1];
    v2 = MILITARY_SHIP_PLUME[1][1];
    plume_array[1] = sf::Vertex((v1*r + v2*(1-r)).to_sfml(), PLUME_COLOUR);
    
    for (int i=0; i<8; i+=2)
    {
        v1 = DASHED_MILITARY_SHIP_PLUME[0][i];
        v2 = DASHED_MILITARY_SHIP_PLUME[1][i];
        dashed_plume_array[i] = sf::Vertex((v1*r + v2*(1-r)).to_sfml(), PLUME_COLOUR);
        v1 = DASHED_MILITARY_SHIP_PLUME[0][i+1];
        v2 = DASHED_MILITARY_SHIP_PLUME[1][i+1];
        dashed_plume_array[i+1] = sf::Vertex((v1*r + v2*(1-r)).to_sfml(), PLUME_COLOUR);
    }
}

void ShipRenderer::take_path_input(Hex h)
{
    if (ship->pathfinding_state == PathfindingState::Computing) return;
    if (pathfinding_ui_state == PathfindUIState::Awaiting)
    {
        goal_position = h;
        pathfinding_ui_state = PathfindUIState::NeedVelocity;
    }
    else if (pathfinding_ui_state == PathfindUIState::NeedVelocity)
    {
        ship->pathfind_to(goal_position, h-goal_position);
        pathfinding_ui_state = PathfindUIState::Awaiting;
    }
}