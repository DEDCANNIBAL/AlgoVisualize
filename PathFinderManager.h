#ifndef ALGOVISUALIZE_PATHFINDERMANAGER_H
#define ALGOVISUALIZE_PATHFINDERMANAGER_H

#include "PathFinder.h"

class PathFinderManager {
    PathFinder *path_finder;
    int delay;
    bool is_going;

    void proceed();

public:
    PathFinderManager();

    void set_algorithm(PathFinder *path_finder);

    void start_in_thread();

    void stop() { is_going = false; }

    void proceed_in_thread();

    void set_delay(float delay);

    bool is_working();
};


#endif //ALGOVISUALIZE_PATHFINDERMANAGER_H
