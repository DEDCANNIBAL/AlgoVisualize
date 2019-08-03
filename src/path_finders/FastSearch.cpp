#include <algorithm>
#include "FastSearch.h"

const int BOOST = 2;

int FastSearch::heuristic(astar_vec2u &cell) {
    return (abs(finish.x - cell.x) +
            abs(finish.y - cell.y)) * BOOST;
}