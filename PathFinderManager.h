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

    bool is_finished();

    void clear() {path_finder->clear();}

    void finish_search() {path_finder->finish_search();}

    const std::shared_ptr<PathFinder> &get_path_finder() const;

    void prepare();

    const std::vector<sf::Vector2u> &get_path() const;

    const std::vector<sf::Vector2u> &find_and_return_path();
};


#endif //ALGOVISUALIZE_PATHFINDERMANAGER_H
