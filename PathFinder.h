#ifndef ALGOVISUALIZE_PATHFINDER_H
#define ALGOVISUALIZE_PATHFINDER_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <climits>
#include <queue>
#include <sys/types.h>
#include "Field.h"


class StopIterationError : public std::exception {
};


class PathFinder {
protected:
    Field &field;
    std::vector<sf::Vector2u> path;
    std::vector<std::vector<uint>> dist;
    sf::Vector2u size, start, finish;
    bool finished;
    const std::vector<sf::Vector2u> shifts = {{0,        1},
                                              {0, UINT_MAX},
                                              {1,        0},
                                              {UINT_MAX, 0}};

public:
    explicit PathFinder(Field &field);

    const std::vector<sf::Vector2u> &get_path() const { return path; }

    virtual void prepare();

    void clear();

    void finish_search();

    bool is_finished() const { return finished; }

    virtual void next() = 0;

    virtual void restore_path();

};


#endif //ALGOVISUALIZE_PATHFINDER_H
