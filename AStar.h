#ifndef ALGOVISUALIZE_ASTAR_H
#define ALGOVISUALIZE_ASTAR_H

#include "PathFinder.h"

struct vector_comparator{

    bool operator()(const sf::Vector2u a, const sf::Vector2u b) const;
};

class AStar : public PathFinder {
    std::priority_queue<sf::Vector2u, std::vector<sf::Vector2u>, vector_comparator> queue;

    void process_adjacent_cells(sf::Vector2u cell);
public:
    explicit AStar(Field &field);

    void prepare() override;

    void next() override;
};

#endif //ALGOVISUALIZE_BREADTHFIRSTSEARCH_H
