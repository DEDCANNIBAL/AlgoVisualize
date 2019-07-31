#ifndef ALGOVISUALIZE_TESTSEARCH_H
#define ALGOVISUALIZE_TESTSEARCH_H


#include <stack>
#include "PathFinder.h"


class TestSearch : public PathFinder {
    std::stack<sf::Vector2u> stack;

    int heuristic(sf::Vector2u &cell);

    void process_adjacent_cells(sf::Vector2u cell);

public:
    explicit TestSearch(Field &field) : PathFinder(field) {}

    void prepare() override;

    void next() override;
};


#endif //ALGOVISUALIZE_TESTSEARCH_H
