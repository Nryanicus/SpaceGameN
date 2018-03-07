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

bool operator<(const PositionVelocityAccelerationValue& lhs, const PositionVelocityAccelerationValue& rhs)
{
    return lhs.value < rhs.value;
}

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
    // return node.acceleration.distance(Hex()) + goal.acceleration.distance(Hex());
    return 1+goal.acceleration.distance(Hex());
}

int heuristic(PositionVelocityAcceleration node, PositionVelocityAcceleration goal)
{
    int dpos = node.position.distance(goal.position);
    // int dvel = node.velocity.distance(goal.velocity);
    return dpos;
}

std::vector<PositionVelocityAcceleration> neighbours(PositionVelocityAcceleration node, std::vector<PlanetoidGameObject*>* planetoids, int acceleration)
{
    std::vector<PositionVelocityAcceleration> ns;
    
    Hex new_vel = node.velocity + node.acceleration;
    Hex new_pos = node.position + new_vel;
    
    // apply gravity to our vector
    for (PlanetoidGameObject* p: *planetoids)
        new_vel += p->get_gravity_at_point(new_pos);

    // do not give vectors which will result in collisions
    for (PlanetoidGameObject* p: *planetoids)
        if (p->collision_in_path(node.position, new_pos))
            return ns;

    // include case with no acceleration
    ns.push_back(PositionVelocityAcceleration(new_pos, new_vel, Hex()));
    // all possible acceleration cases
    for (Hex h: CARDINAL_DIRECTIONS)
        ns.push_back(PositionVelocityAcceleration(new_pos, new_vel, h*acceleration));
    return ns;
}

void insert(std::deque<PositionVelocityAccelerationValue>* frontier, PositionVelocityAccelerationValue value)
{
    std::deque<PositionVelocityAccelerationValue>::iterator it = std::lower_bound( frontier->begin(), frontier->end(), value);
    frontier->insert(it, value);
}

std::deque<Hex> pathfind(Hex start_pos, Hex start_vel, Hex goal_pos, Hex goal_vel, int acceleration, std::vector<PlanetoidGameObject*>* planetoids)
{
    // set up initial variables

    // use beam search if goal is distant
    // bool beam_search = start_pos.distance(goal_pos) + start_vel.distance(goal_vel) > BEAM_SEARCH_CUTOFF;
    bool beam_search = true;

    PositionVelocityAcceleration start(start_pos, start_vel, Hex());
    PositionVelocityAcceleration goal(goal_pos, goal_vel, Hex());

    std::deque<PositionVelocityAccelerationValue> frontier;
    frontier.push_back(PositionVelocityAccelerationValue(start, heuristic(start, goal)));

    std::unordered_map<PositionVelocityAcceleration, PositionVelocityAcceleration> came_from;

    std::unordered_map<PositionVelocityAcceleration, int> cost_so_far;
    cost_so_far[start] = 0;

    int nodes_searched = 0;
    int nodes_pruned = 0;
    // std::cout << "finding path from " << start_pos << " " << start_vel << " to " << goal_pos << " " << goal_vel << std::endl;

    while (!frontier.empty() && nodes_searched < MAX_SEARCH)
    {
        nodes_searched++;
        PositionVelocityAcceleration current = frontier.front().positionvelocityacceleration;
        // std::cout << "searching " << current.position << " " << current.velocity << " " << frontier.front().value << std::endl;
        frontier.pop_front();
        // we've found our goal, return
        if (current == goal)
        {
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
            // trimming initial zero-vel and -accel states
            std::deque<Hex> acceleration_path;
            // std::cout << "planned states" << std::endl;
            bool dead_initial_states = true;
            int fuel_cost = 0;
            for (int i=pva_path.size()-1; i>0; i--)
            {
                if (dead_initial_states && (pva_path[i].velocity + pva_path[i].acceleration == Hex()))
                    continue;
                else
                    dead_initial_states = false;
                // std::cout << pva_path[i].position << " " << pva_path[i].velocity + pva_path[i].acceleration << std::endl;
                acceleration_path.push_back(pva_path[i].acceleration);
                fuel_cost += pva_path[i].acceleration.distance(Hex());
            }
            std::cout << "goal found in " << nodes_searched << " steps, with " << nodes_pruned << " nodes pruned, length: " << pva_path.size() << " fuel cost: " << fuel_cost << std::endl;
            // if (beam_search)
            //     std::cout << "using beam search" << std::endl;

            // delete unneeded data
            for (PlanetoidGameObject* p: *planetoids)
                delete p;
            delete planetoids;

            return acceleration_path;
        }

        // update paths and add new nodes to frontier as appropriate
        for (PositionVelocityAcceleration n: neighbours(current, planetoids, acceleration))
        {
            int new_cost = cost_so_far[current] + graph_cost(current, n);
            if (cost_so_far.count(n) == 0 || new_cost < cost_so_far[n])
            {
                cost_so_far[n] = new_cost;
                int priority = new_cost + heuristic(n, goal);
                // std::cout << "adding neighbour " << n.position << " " << n.velocity << " " << priority << std::endl;
                insert(&frontier, PositionVelocityAccelerationValue(n, priority));
                came_from[n] = current;
            }
        }

        // prune frontier until only BEAM_WIDTH elements remain
        if (beam_search)
            while (frontier.size() > BEAM_WIDTH)
            {
                frontier.pop_back();
                nodes_pruned++;
            }
    }
    // we failed to find a path
    std::cout << "goal not found in " << nodes_searched << " steps, with " << nodes_pruned << " nodes pruned" << std::endl;
    return std::deque<Hex>();
}