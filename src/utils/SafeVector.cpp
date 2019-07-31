#include "SafeVector.h"


void SafeCellsVector::clear() {
    cells_mutex.lock();

    cells.clear();

    cells_mutex.unlock();
}

void SafeCellsVector::push_back(sf::Vector2u cell) {
    cells_mutex.lock();

    cells.push_back(cell);

    cells_mutex.unlock();
}


std::vector<sf::Vector2u> SafeCellsVector::get_vector() {
    cells_mutex.lock();

    auto res = cells;

    cells_mutex.unlock();

    return res;
}
