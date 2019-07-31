#ifndef ALGOVISUALIZE_FASTSEARCH_H
#define ALGOVISUALIZE_FASTSEARCH_H

#include <stack>
#include "AStar.h"


class FastSearch : public AStar {
    int heuristic(astar_vec2u &cell) override;


public:
    explicit FastSearch(Field &field) : AStar(field) {}
};


#endif //ALGOVISUALIZE_FASTSEARCH_H
