#include "PathFinderManager.h"
#include <chrono>
#include <thread>


PathFinderManager::PathFinderManager(Field &field) :
        path_finder(nullptr),
        delay(0),
        is_going(false),
        field(field)
        {}

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
        std::this_thread::sleep_for(std::chrono::microseconds(delay));
    }
    stop();
}

void PathFinderManager::set_delay(float delay) {
    this->delay = static_cast<int> (delay * 1e6);
}

bool PathFinderManager::is_working() {
    return not path_finder->is_finished();
}

const std::shared_ptr<PathFinder> &PathFinderManager::get_path_finder() const {
    return path_finder;
}

void PathFinderManager::prepare() {
    path_finder->prepare();
}

bool PathFinderManager::is_finished() {
    return path_finder->is_finished();
}

const std::vector<sf::Vector2u> &PathFinderManager::get_path() const {
    return path_finder->get_path();
}
