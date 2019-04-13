#ifndef ALGOVISUALIZE_PATHFINDER_H
#define ALGOVISUALIZE_PATHFINDER_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <climits>
#include <queue>
#include <sys/types.h>
#include "Field.h"


class PathFinder {
    Field &field;
    std::vector<sf::Vector2u> path;
    bool finished, is_going;
    sf::Vector2u size, start, finish;
    std::queue<sf::Vector2u> queue;
    std::vector<std::vector<uint>> dist;
    const std::vector<sf::Vector2u> shifts = {{0, 1},
                                             {0, UINT_MAX},
                                             {1, 0},
                                             {UINT_MAX, 0}};

    void prepare();

    void restore_path();

    void process_adj_cells(sf::Vector2u cell);

public:
    explicit PathFinder(Field &field);

    void bfs();

    bool is_finished() const { return finished; }

    void stop() { is_going = false; }

    const std::vector<sf::Vector2u> &get_path() const { return path; }

};


#endif //ALGOVISUALIZE_PATHFINDER_H
