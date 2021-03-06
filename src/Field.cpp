#include <random>
#include <ctime>
#include "Field.h"


bool operator<(const sf::Vector2u &a, const sf::Vector2u &b) {
    return a.x < b.x && a.y < b.y;
}

Field::Field(sf::Vector2u size) :
        size(size),
        obstacles(size.x, std::vector<u_int8_t>(size.y, Cell::Empty)),
        visited(size.x, std::vector<bool>(size.y, false)),
        visited_cells(0){
    set_start({0, size.y / 2});
    set_finish({size.x - 1, size.y / 2});
}

u_int8_t Field::visit_cell(sf::Vector2u pos) {
    if (pos < size and not visited[pos.x][pos.y]) {
        visited_cells++;
        visited[pos.x][pos.y] = true;
        change_cell(pos);
    }
    return get_cell(pos);
}

void Field::reset_visited() {
    visited.assign(size.x, std::vector<bool>(size.y, false));
    change_all_cells();
    visited_cells = 0;
}

uint8_t Field::get_cell(sf::Vector2u pos) const {
    return pos < size ? obstacles[pos.x][pos.y] : Cell::Wall;
}

void Field::clear_obstacles() {
    obstacles.assign(size.x, std::vector<u_int8_t>(size.y, Cell::Empty));
    set_start(start);
    set_finish(finish);
    change_all_cells();
}

void Field::set_start(sf::Vector2u start) {
    set_cell(this->start, Cell::Empty);
    this->start = start;
    set_cell(start, Cell::Start);
}

void Field::set_finish(sf::Vector2u finish) {
    set_cell(this->finish, Cell::Empty);
    this->finish = finish;
    set_cell(finish, Cell::Finish);
}

void Field::set_cell(sf::Vector2u pos, u_int8_t cell) {
    if (pos < size) {
        obstacles[pos.x][pos.y] = cell;
        change_cell(pos);
    }
}

std::vector<sf::Vector2u> Field::observe_changed_cells() {
    std::vector<sf::Vector2u> res = changed_cells.pop_vector();
    return std::move(res);
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

int Field::get_visited_cells() const {
    return visited_cells;
}
