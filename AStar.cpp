#include "AStar.h"
#include <algorithm>


void AStar::prepare() {
    PathFinder::prepare();
    while (!queue.empty())
        queue.pop();
    queue.push({0, start});
}

void AStar::next() {
    if (is_finished())
        throw StopIterationError();
    auto [rate, current_cell] = queue.top();
    queue.pop();
    if (current_cell == finish) {
        restore_path();
        finished = true;
        return;
    }
    process_adjacent_cells(current_cell);
    if (queue.empty())
        finished = true;
}

AStar::AStar(Field &field) :
        PathFinder(field) {

}

void AStar::process_adjacent_cells(astar_vec2u cell) {
    for (auto &shift : shifts) {
        astar_vec2u adjacent_cell = cell + shift;
        if (field.visit_cell(adjacent_cell) != Cell::Wall &&
            dist[adjacent_cell.x][adjacent_cell.y] > dist[cell.x][cell.y] + 1) {
            dist[adjacent_cell.x][adjacent_cell.y] = dist[cell.x][cell.y] + 1;
            queue.push({
                -dist[adjacent_cell.x][adjacent_cell.y] - heuristic(adjacent_cell),
                adjacent_cell
            });
        }
    }
}

int AStar::heuristic(astar_vec2u &cell) {
    return abs(finish.x - cell.x) +
           abs(finish.y - cell.y);
}
