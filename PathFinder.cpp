#include "PathFinder.h"

#include <algorithm>
#include <thread>
#include <chrono>


PathFinder::PathFinder(Field &field) :
        field(field),
        finished(true),
        is_going(false),
        size(field.get_size()) {
    set_delay(0);
}

void PathFinder::bfs() {
    prepare();
    int iteration_size = 1;
    while (iteration_size and is_going){
        iteration_size = bfs_iterate_once(iteration_size);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    finished = true;
    is_going = false;
}

int PathFinder::bfs_iterate_once(int iteration_size) {
    int next_iteration_size = 0;

    for (int i = 0; i < iteration_size and is_going; i++) {
        sf::Vector2u current_cell = queue.front();
        queue.pop();
        if (current_cell == finish) {
            restore_path();
            is_going = false;
            break;
        }
        next_iteration_size += process_adj_cells(current_cell);
    }

    return next_iteration_size;
}

void PathFinder::prepare() {
    field.reset_visited();
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

int PathFinder::process_adj_cells(sf::Vector2u cell) {
    int added_cells = 0;
    for (auto &shift : shifts) {
        sf::Vector2u adj_cell = cell + shift;
        if (field.visit_cell(adj_cell) != Cell::Wall &&
            dist[adj_cell.x][adj_cell.y] > dist[cell.x][cell.y] + 1) {
            dist[adj_cell.x][adj_cell.y] = dist[cell.x][cell.y] + 1;
            queue.push(adj_cell);
            added_cells++;
        }
    }

    return added_cells;
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
