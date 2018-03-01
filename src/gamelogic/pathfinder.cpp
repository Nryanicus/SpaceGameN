#include "pathfinder.hpp"

class PositionVelocityAcceleration
{
public:
    Hex position, velocity, acceleration;

    PositionVelocityAcceleration()
    : position(Hex()), velocity(Hex()), acceleration(Hex())
    {}

    PositionVelocityAcceleration(Hex p, Hex v, Hex a)
    : position(p), velocity(v), acceleration(a)
    {}

    bool operator==(PositionVelocityAcceleration o)
    {
        return position == o.position && velocity == o.velocity && acceleration == o.acceleration;
    }
};

const bool operator==(const PositionVelocityAcceleration& lhs, const PositionVelocityAcceleration& rhs)
{
    return lhs.position == rhs.position && lhs.velocity == rhs.velocity && lhs.acceleration == rhs.acceleration;
}

class PositionVelocityAccelerationValue
{
public:
    PositionVelocityAcceleration positionvelocityacceleration;
    int value;
    
    PositionVelocityAccelerationValue(PositionVelocityAcceleration pva, int v)
    : positionvelocityacceleration(pva), value(v)
    {}
    
    bool operator==(PositionVelocityAccelerationValue o)
    {
        return positionvelocityacceleration == o.positionvelocityacceleration && value == o.value;
    }
};

class PositionVelocityAccelerationValueComparison
{
public:
    bool operator()(const PositionVelocityAccelerationValue& lhs, const PositionVelocityAccelerationValue& rhs)
    {
        return lhs.value > rhs.value;
    }
};

// hash functions
namespace std
{
    template<> struct hash<PositionVelocityAcceleration>
    {
        typedef PositionVelocityAcceleration argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& pva) const
        {
            result_type const pva1 ( std::hash<Hex>()(pva.position) );
            result_type const pva2 ( std::hash<Hex>()(pva.velocity) );
            result_type const pva3 ( std::hash<Hex>()(pva.velocity) );
            return ((pva1 ^ (pva2 << 1)) ^ (pva3 << 1)); 
        }
    }; 
    template<> struct hash<PositionVelocityAccelerationValue>
    {
        typedef PositionVelocityAccelerationValue argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& pvv) const
        {
            result_type const pvv1 ( std::hash<PositionVelocityAcceleration>()(pvv.positionvelocityacceleration) );
            result_type const pvv2 ( std::hash<int>()(pvv.value) );
            return pvv1 ^ (pvv2 << 1); 
        }
    };
}


int graph_cost(PositionVelocityAcceleration node, PositionVelocityAcceleration goal)
{
    // fuel conservation
    return 4 + node.acceleration.distance(Hex()) + goal.acceleration.distance(Hex());// + heuristic(node, goal);
}

int heuristic(PositionVelocityAcceleration node, PositionVelocityAcceleration goal)
{
    int dpos = node.position.distance(goal.position);
    int dvel = node.velocity.distance(goal.velocity);

    return dpos; 
}

std::vector<PositionVelocityAcceleration> neighbours(PositionVelocityAcceleration node, std::vector<Planetoid*>* planetoids, int max_acceleration)
{
    std::vector<PositionVelocityAcceleration> ns;
    
    Hex new_pos = node.position + node.velocity;
    Hex new_vel = node.velocity + node.acceleration;
    
    // apply gravity to our vector
    for (Planetoid* p: *planetoids)
        new_vel += p->get_gravity_at_point(new_pos);
    // do not give vectors which will result in collisions
    Hex col;
    for (Planetoid* p: *planetoids)
        if (p->collision_in_path(new_pos, new_pos+new_vel, &col))
            return ns;

    // include case with no acceleration
    ns.push_back(PositionVelocityAcceleration(new_pos, new_vel, Hex()));
    // all possible acceleration cases
    for (int a=1; a<=max_acceleration; a++)
        for (Hex h: CARDINAL_DIRECTIONS)
            ns.push_back(PositionVelocityAcceleration(new_pos, new_vel, h*a));
    return ns;
}

std::deque<Hex> pathfind(Hex start_pos, Hex start_vel, Hex goal_pos, Hex goal_vel, int max_acceleration, std::vector<Planetoid*>* planetoids)
{
    assert(max_acceleration > 0);
    // set up initial variables
    // since acceleration affects the turn ahead, begin pathfinding the step before (it will be ignored when reconstructing the path)
    PositionVelocityAcceleration start(start_pos, start_vel, Hex());
    PositionVelocityAcceleration goal(goal_pos, goal_vel, Hex());

    std::priority_queue<PositionVelocityAccelerationValue, std::vector<PositionVelocityAccelerationValue>, PositionVelocityAccelerationValueComparison> frontier;
    frontier.push(PositionVelocityAccelerationValue(start, heuristic(start, goal)));

    std::unordered_map<PositionVelocityAcceleration, PositionVelocityAcceleration> came_from;

    std::unordered_map<PositionVelocityAcceleration, int> cost_so_far;
    cost_so_far[start] = 0;

    int nodes_searched = 0;
    // std::cout << "finding path from " << start_pos << " " << start_vel << " to " << goal_pos << " " << goal_vel << std::endl;

    while (!frontier.empty())
    {
        nodes_searched++;
        PositionVelocityAcceleration current = frontier.top().positionvelocityacceleration;
        // std::cout << "searching " << current.position << " " << current.velocity << " " << frontier.top().value << std::endl;
        frontier.pop();
        // we've found our goal, return
        if (current == goal)
        {
            // std::cout << "goal found in " << nodes_searched << " steps, creating acceleration path" << std::endl;
            std::vector<PositionVelocityAcceleration> pva_path;
            pva_path.push_back(current);
            // std::cout << current.position << " " << current.velocity << " " << current.acceleration << std::endl;
            while (came_from.count(current))
            {
                // std::cout << current.position << " " << current.velocity << " " << current.acceleration << std::endl;
                current = came_from[current];
                pva_path.push_back(current);
            }
            // std::cout << pva_path.size() << std::endl;
            // create list of accelerations needed to follow found path
            // ignoring last elements as it'll be zero acceleration
            std::deque<Hex> acceleration_path;
            for (int i=pva_path.size()-2; i>0; i--)
                acceleration_path.push_back(pva_path[i].acceleration);
            return acceleration_path;
        }

        // update paths and add new nodes to frontier as appropriate
        for (PositionVelocityAcceleration n: neighbours(current, planetoids, max_acceleration))
        {
            int new_cost = cost_so_far[current] + graph_cost(current, n);
            if (cost_so_far.count(n) == 0 || new_cost < cost_so_far[n])
            {
                cost_so_far[n] = new_cost;
                int priority = new_cost + heuristic(n, goal);
                // std::cout << "adding neighbour " << n.position << " " << n.velocity << " " << priority << std::endl;
                // std::cout << priority << " ";
                frontier.push(PositionVelocityAccelerationValue(n, priority));
                came_from[n] = current;
            }
        }
    }
    // we failed to find a path
    return std::deque<Hex>();
}