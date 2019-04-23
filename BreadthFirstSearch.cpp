#include "BreadthFirstSearch.h"
#include <algorithm>


void BreadthFirstSearch::prepare() {
    PathFinder::prepare();
    while (!queue.empty())
        queue.pop();
    queue.push(start);
}

void BreadthFirstSearch::next() {
    if(is_finished())
        throw StopIterationError();
    auto current_cell = queue.front();
    queue.pop();
    if (current_cell == finish) {
        restore_path();
        finished = true;
    }
    process_adjacent_cells(current_cell);
    if(queue.empty())
        finished = true;
}

void BreadthFirstSearch::process_adjacent_cells(sf::Vector2u cell) {
    for (auto &shift : shifts) {
        sf::Vector2u adjacent_cell = cell + shift;
        if (field.visit_cell(adjacent_cell) != Cell::Wall &&
            dist[adjacent_cell.x][adjacent_cell.y] > dist[cell.x][cell.y] + 1) {
            dist[adjacent_cell.x][adjacent_cell.y] = dist[cell.x][cell.y] + 1;
            queue.push(adjacent_cell);
        }
    }
}
