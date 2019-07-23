#include "AStar.h"
#include <algorithm>


void AStar::prepare() {
    PathFinder::prepare();
    while (!queue.empty())
        queue.pop();
    queue.push(start);
}

void AStar::next() {
    if(is_finished())
        throw StopIterationError();
    auto current_cell = queue.top();
    queue.pop();
    if (current_cell == finish) {
        restore_path();
        finished = true;
    }
    process_adjacent_cells(current_cell);
    if(queue.empty())
        finished = true;
}

AStar::AStar(Field &field) :
    PathFinder(field),
    queue(vector_comparator()){

}

bool vector_comparator::operator()(const sf::Vector2u a, const sf::Vector2u b) const {
    return false;
}

void AStar::process_adjacent_cells(sf::Vector2u cell) {
    for (auto &shift : shifts) {
        sf::Vector2u adjacent_cell = cell + shift;
        if (field.visit_cell(adjacent_cell) != Cell::Wall &&
            dist[adjacent_cell.x][adjacent_cell.y] > dist[cell.x][cell.y] + 1) {
            dist[adjacent_cell.x][adjacent_cell.y] = dist[cell.x][cell.y] + 1;
            queue.push(adjacent_cell);
        }
    }
}
