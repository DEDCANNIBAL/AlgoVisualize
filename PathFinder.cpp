#include "PathFinder.h"

#include <algorithm>


PathFinder::PathFinder(Field &field) :
        field(field),
        finished(true),
        is_going(false),
        size(field.get_size()) {}

void PathFinder::prepare() {
    start = field.get_start();
    finish = field.get_finish();
    finished = false;
    is_going = true;
    while (!queue.empty())
        queue.pop();
    path.clear();
    dist.assign(size.x, std::vector<uint>(size.y, UINT_MAX - 1));
    queue.push(start);
    dist[start.x][start.y] = 0;
}

void PathFinder::restore_path() {
    sf::Vector2u current_cell = finish;
    path.push_back(finish);
    while (current_cell != start)
        for (auto &shift : shifts) {
            sf::Vector2u adj_cell = current_cell + shift;
            if (adj_cell < size && dist[adj_cell.x][adj_cell.y] + 1 == dist[current_cell.x][current_cell.y]) {
                current_cell = adj_cell;
                path.push_back(adj_cell);
                break;
            }
        }
    std::reverse(path.begin(), path.end());
}

void PathFinder::process_adj_cells(sf::Vector2u cell) {
    for (auto &shift : shifts) {
        sf::Vector2u adj_cell = cell + shift;
        if (field.visit_cell(adj_cell) != Cell::Wall &&
            dist[adj_cell.x][adj_cell.y] > dist[cell.x][cell.y] + 1) {
            dist[adj_cell.x][adj_cell.y] = dist[cell.x][cell.y] + 1;
            queue.push(adj_cell);
        }
    }
}

void PathFinder::bfs() {
    prepare();
    while (!queue.empty() && is_going) {
        sf::Vector2u current_cell = queue.front();
        queue.pop();
        if (current_cell == finish) {
            restore_path();
            break;
        }
        process_adj_cells(current_cell);
    }
    finished = true;
    is_going = false;
}

