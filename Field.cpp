#include <random>
#include <ctime>
#include "Field.h"


bool operator<(const sf::Vector2u &a, const sf::Vector2u &b) {
    return a.x < b.x && a.y < b.y;
}

Field::Field(sf::Vector2u size) :
        size(size),
        obstacles(size.x, std::vector<u_int8_t>(size.y, Cell::Empty)),
        visited(size.x, std::vector<bool>(size.y, false)) {
    set_start({0, size.y / 2});
    set_finish({size.x - 1, size.y / 2});
}

void Field::set_cell(sf::Vector2u pos, u_int8_t cell) {
    if (pos < size and
        pos != start and
        pos != finish) {
        obstacles[pos.x][pos.y] = cell;
        change_cell(pos);
    }
}

u_int8_t Field::visit_cell(sf::Vector2u pos) {
    if (pos < size) {
        visited[pos.x][pos.y] = true;
        change_cell(pos);
    }
    return get_cell(pos);
}

void Field::reset_visited() {
    visited.assign(size.x, std::vector<bool>(size.y, false));
    change_all_cells();
}

uint8_t Field::get_cell(sf::Vector2u pos) const {
    return pos < size ? obstacles[pos.x][pos.y] : Cell::Wall;
}

void Field::clear_obstacles() {
    obstacles.assign(size.x, std::vector<u_int8_t>(size.y, Cell::Empty));
    change_all_cells();
}

void Field::randomize() {
    std::default_random_engine gen(time(0));
    const static float wall_percent = 0.45;
    for (int i = 0; i < size.x * size.y * wall_percent; i++) {
        uint x = gen() % size.x;
        uint y = gen() % size.y;
        sf::Vector2u pos(x, y);
        set_cell(pos, Cell::Wall);
    }
}

void Field::set_start(sf::Vector2u start) {
    set_cell(this->start, Cell::Empty);
    set_cell(start, Cell::Start);
    this->start = start;
}

void Field::set_finish(sf::Vector2u finish) {
    set_cell(this->finish, Cell::Empty);
    set_cell(finish, Cell::Finish);
    this->finish = finish;
}

std::vector<sf::Vector2u> Field::observe_changed_cells() {
    std::vector<sf::Vector2u> res = changed_cells.get_vector();
    changed_cells.clear();
    return res;
}

void Field::change_cell(sf::Vector2u pos) {
    changed_cells.push_back(pos);
}

void Field::change_all_cells() {
    changed_cells.clear();
    for (uint i = 0; i < size.x; i++)
        for (uint j = 0; j < size.y; j++)
            change_cell({i, j});
}
