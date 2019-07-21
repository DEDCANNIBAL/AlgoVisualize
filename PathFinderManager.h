#ifndef ALGOVISUALIZE_PATHFINDERMANAGER_H
#define ALGOVISUALIZE_PATHFINDERMANAGER_H

#include <memory>
#include "PathFinder.h"

class PathFinderManager {
    std::shared_ptr<PathFinder> path_finder;
    int delay;
    bool is_going;
    Field &field;

    void proceed();

public:
    PathFinderManager(Field&);

    template <class PathFinderT>
    void set_algorithm(){
        path_finder = std::static_pointer_cast<PathFinder>(std::make_shared<PathFinderT>(field));

        stop();
    };

    void start_in_thread();

    void stop() { is_going = false; }

    void proceed_in_thread();

    void set_delay(float delay);

    bool is_working();

    const std::shared_ptr<PathFinder> &get_path_finder() const;
};


#endif //ALGOVISUALIZE_PATHFINDERMANAGER_H
