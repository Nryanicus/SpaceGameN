#include "pathfinder.hpp"

class PositionVelocity
{
public:
    Hex position, velocity;

    PositionVelocity()
    : position(Hex()), velocity(Hex())
    {}

    PositionVelocity(Hex p, Hex v)
    : position(p), velocity(v)
    {}

    bool operator==(PositionVelocity o)
    {
        return position == o.position && velocity == o.velocity;
    }
};

const bool operator==(const PositionVelocity& lhs, const PositionVelocity& rhs)
{
    return lhs.position == rhs.position && lhs.velocity == rhs.velocity;
}

class PositionVelocityValue
{
public:
    PositionVelocity positionvelocity;
    int value;
    
    PositionVelocityValue(PositionVelocity pv, int v)
    : positionvelocity(pv), value(v)
    {}
    
    bool operator==(PositionVelocityValue o)
    {
        return positionvelocity == o.positionvelocity && value == o.value;
    }
};

class PositionVelocityValueComparison
{
public:
    bool operator()(const PositionVelocityValue& lhs, const PositionVelocityValue& rhs)
    {
        return lhs.value < rhs.value;
    }
};

// hash functions
namespace std
{
    template<> struct hash<PositionVelocity>
    {
        typedef PositionVelocity argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& pv) const
        {
            result_type const pv1 ( std::hash<Hex>()(pv.position) );
            result_type const pv2 ( std::hash<Hex>()(pv.velocity) );
            return pv1 ^ (pv2 << 1); 
        }
    }; 
    template<> struct hash<PositionVelocityValue>
    {
        typedef PositionVelocityValue argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& pvv) const
        {
            result_type const pvv1 ( std::hash<PositionVelocity>()(pvv.positionvelocity) );
            result_type const pvv2 ( std::hash<int>()(pvv.value) );
            return pvv1 ^ (pvv2 << 1); 
        }
    };
}

int heuristic(PositionVelocity node, PositionVelocity goal)
{
    return node.position.distance(goal.position) + node.velocity.distance(goal.velocity);
}

std::vector<PositionVelocity> neighbours(PositionVelocity node, std::vector<Planetoid> planetoids, int max_acceleration)
{
    std::vector<PositionVelocity> ns;
    Hex new_pos = node.position + node.velocity;
    for (int a=1; a<=max_acceleration; a++)
        for (Hex h: CARDINAL_DIRECTIONS)
        {
            Hex new_vel = node.velocity + h*a;
            ns.push_back(PositionVelocity(new_pos, new_vel));
        }
    return ns;
}

std::vector<Hex> pathfind(Hex start_pos, Hex start_vel, Hex goal_pos, Hex goal_vel, int max_acceleration, std::vector<Planetoid> planetoids)
{
    assert(max_acceleration > 0);
    // set up initial variables
    PositionVelocity start(start_pos, start_vel);
    PositionVelocity goal(goal_pos, goal_vel);

    std::priority_queue<PositionVelocityValue, std::vector<PositionVelocityValue>, PositionVelocityValueComparison> frontier;
    frontier.push(PositionVelocityValue(start, heuristic(start, goal)));

    std::unordered_map<PositionVelocity, PositionVelocity> came_from;

    std::unordered_map<PositionVelocity, int> cost_so_far;
    cost_so_far[start] = 0;

    while (!frontier.empty())
    {
        PositionVelocity current = frontier.top().positionvelocity;
        frontier.pop();
        // we've found our goal, return
        if (current == goal)
        {
            std::vector<PositionVelocity> pv_path;
            pv_path.push_back(current);
            while (came_from.count(current))
            {
                current = came_from[current];
                pv_path.push_back(current);
            }
            // create list of accelerations needed to follow found path
            std::vector<Hex> acceleration_path;
            for (int i=pv_path.size()-2; i>=0; i++)
                acceleration_path.push_back(pv_path[i].velocity - pv_path[i+1].velocity);
            return acceleration_path;
        }

        // update paths and add new nodes to frontier as appropriate
        for (PositionVelocity n: neighbours(current, planetoids, max_acceleration))
        {
            int new_cost = cost_so_far[current] + heuristic(current, n);
            if (cost_so_far.count(n) == 0 || new_cost < cost_so_far[n])
            {
                cost_so_far[n] = new_cost;
                int priority = new_cost + heuristic(n, goal);
                frontier.push(PositionVelocityValue(n, priority));
                came_from[n] = current;
            }
        }
    }
    // we failed to find a path
    return std::vector<Hex>();
}