#include <random>
#include <ctime>
#include "Field.h"


bool operator<(const sf::Vector2u &a, const sf::Vector2u &b) {
    return a.x < b.x && a.y < b.y;
}

Field::Field(sf::Vector2u size) :
        size(size),
        obstacles(size.x, std::vector<bool>(size.y, Cell::Empty)),
        visited(size.x, std::vector<bool>(size.y, Cell::Empty)),
        start(0, size.y / 2),
        finish(sf::Vector2u(size.x - 1, size.y / 2)) {}

void Field::set_cell(sf::Vector2u pos, bool cell) {
    if (pos < size)
        obstacles[pos.x][pos.y] = cell;
}

bool Field::visit_cell(sf::Vector2u pos) {
    if (pos < size)
        visited[pos.x][pos.y] = true;
    return get_cell(pos);
}

void Field::reset_visited() {
    visited.assign(size.x, std::vector<bool>(size.y, Cell::Empty));
}

bool Field::get_cell(sf::Vector2u pos) const {
    return !(pos < size) || obstacles[pos.x][pos.y];
}

void Field::clear_obstacles() {
    obstacles.assign(size.x, std::vector<bool>(size.y, Cell::Empty));
}

void Field::randomize() {
    std::default_random_engine gen(time(0));
    const static float wall_percent = 0.45;
    for (int i = 0; i < size.x * size.y * wall_percent; i++) {
        uint x = gen() % size.x;
        uint y = gen() % size.y;
        sf::Vector2u pos(x, y);
        if (pos != start && pos != finish)
            set_cell(pos, Cell::Wall);
    }
}

void Field::set_start(sf::Vector2u start) {
    if (get_cell(start) == Cell::Empty)
        this->start = start;
}

void Field::set_finish(sf::Vector2u finish) {
    if (get_cell(finish) == Cell::Empty)
        this->finish = finish;
}
