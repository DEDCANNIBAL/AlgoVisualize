#include "PathFinderManager.h"
#include <chrono>
#include <thread>


PathFinderManager::PathFinderManager() :
        path_finder(nullptr),
        delay(0),
        is_going(false) {

}

void PathFinderManager::set_algorithm(PathFinder *path_finder) {
    this->path_finder = path_finder;
    stop();
}

void PathFinderManager::proceed_in_thread() {
    if(is_going)
        return;
    is_going = true;
    std::thread path_finding_thread(&PathFinderManager::proceed, this);
    path_finding_thread.detach();
}

void PathFinderManager::start_in_thread() {
    path_finder->prepare();
    proceed_in_thread();
}

void PathFinderManager::proceed() {
    while (is_going and not path_finder->is_finished()) {
        path_finder->next();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    stop();
}

void PathFinderManager::set_delay(float delay) {
    this->delay = static_cast<int> (delay * 1e3);
}

bool PathFinderManager::is_working() {
    return not path_finder->is_finished();
}
