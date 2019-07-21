#ifndef ALGOVISUALIZE_ASTAR_H
#define ALGOVISUALIZE_ASTAR_H

#include "PathFinder.h"

class AStar : public PathFinder {
    std::queue<sf::Vector2u> queue;

    void process_adjacent_cells(sf::Vector2u cell);

public:
    explicit AStar(Field &field) : PathFinder(field) {}

    void prepare() override;

    void next() override;

};


#endif //ALGOVISUALIZE_BREADTHFIRSTSEARCH_H
