#ifndef ALGOVISUALIZE_BREADTHFIRSTSEARCH_H
#define ALGOVISUALIZE_BREADTHFIRSTSEARCH_H

#include "PathFinder.h"

class BreadthFirstSearch : public PathFinder {
    std::queue<sf::Vector2u> queue;

    void process_adjacent_cells(sf::Vector2u cell);

public:
    explicit BreadthFirstSearch(Field &field) : PathFinder(field) {}

    void prepare() override;

    void next() override;

};


#endif //ALGOVISUALIZE_BREADTHFIRSTSEARCH_H
