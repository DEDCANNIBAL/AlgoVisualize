#ifndef ALGOVISUALIZE_ASTAR_H
#define ALGOVISUALIZE_ASTAR_H

#include "PathFinder.h"


struct astar_vec2u : public sf::Vector2u{
    astar_vec2u(const sf::Vector2u &vector) : sf::Vector2u(vector) {}

    astar_vec2u(unsigned int x, unsigned int y) : sf::Vector2u(x, y) {}

    astar_vec2u() {}

    inline bool operator<(astar_vec2u &o){return false;}
};

class AStar : public PathFinder {
    std::priority_queue<std::pair<int, astar_vec2u>> queue;

    void process_adjacent_cells(astar_vec2u cell);

    virtual int heuristic(astar_vec2u &cell);
public:
    explicit AStar(Field &field);

    void prepare() override;

    void next() override;
};

#endif //ALGOVISUALIZE_BREADTHFIRSTSEARCH_H
