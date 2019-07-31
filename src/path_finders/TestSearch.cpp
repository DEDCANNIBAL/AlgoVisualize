#include <algorithm>
#include "TestSearch.h"


int TestSearch::heuristic(sf::Vector2u &cell) {
    return std::max(abs(finish.x - cell.x),
           abs(finish.y - cell.y));
}

void TestSearch::prepare() {
    PathFinder::prepare();
    while (!stack.empty())
        stack.pop();
    stack.push(start);
}

void TestSearch::next() {
    if (is_finished())
        throw StopIterationError();
    auto current_cell = stack.top();
    stack.pop();
    if (current_cell == finish) {
        restore_path();
        finished = true;
        return;
    }
    process_adjacent_cells(current_cell);
    if (stack.empty())
        finished = true;
}

void TestSearch::process_adjacent_cells(sf::Vector2u cell) {
    std::vector<std::pair<int, int>> heuristic_by_shift;
    for (int i = 0; i < shifts.size(); ++i) {
        sf::Vector2u adjacent_cell = cell + shifts[i];
        heuristic_by_shift.push_back({-heuristic(adjacent_cell), i});
    }
    std::sort(heuristic_by_shift.begin(), heuristic_by_shift.end());
    for(auto [_, i]: heuristic_by_shift){
        sf::Vector2u adjacent_cell = cell + shifts[i];
        if (field.visit_cell(adjacent_cell) != Cell::Wall &&
            dist[adjacent_cell.x][adjacent_cell.y] > dist[cell.x][cell.y] + 1) {
            dist[adjacent_cell.x][adjacent_cell.y] = dist[cell.x][cell.y] + 1;
            stack.push(adjacent_cell);
        }
    }
}