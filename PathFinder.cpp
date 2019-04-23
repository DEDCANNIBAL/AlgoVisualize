#include "PathFinder.h"
#include <algorithm>


PathFinder::PathFinder(Field &field) :
        field(field),
        finished(true),
        size(field.get_size()) {}

void PathFinder::restore_path() {
    sf::Vector2u current_cell = finish;
    path.push_back(finish);
    while (current_cell != start)
        for (auto &shift : shifts) {
            sf::Vector2u adjacent_cell = current_cell + shift;
            if (adjacent_cell < size
                and dist[adjacent_cell.x][adjacent_cell.y] + 1 ==
                    dist[current_cell.x][current_cell.y]) {
                current_cell = adjacent_cell;
                path.push_back(adjacent_cell);
                break;
            }
        }
    std::reverse(path.begin(), path.end());
}

void PathFinder::prepare() {
    field.reset_visited();
    start = field.get_start();
    finish = field.get_finish();
    finished = false;
    path.clear();
    dist.assign(size.x, std::vector<uint>(size.y, UINT_MAX - 1));
    dist[start.x][start.y] = 0;
}

