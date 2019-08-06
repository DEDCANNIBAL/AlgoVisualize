#include <unordered_map>

#include "EllerMazeGenerator.h"
#include "../utils/utils.h"

const float WALL_PERCENT = 0.7;


void EllerMazeGenerator::prepare() {
    MazeGenerator::prepare();
    numbers_of_sets = 0;
    for (uint i = 0; i < size.y; i++)
        set_map[i] = numbers_of_sets++;
    current_line = 0;
}

EllerMazeGenerator::EllerMazeGenerator(Field &field) :
        MazeGenerator(field),
        size(field.get_size()), set_map(size.y) {}

void EllerMazeGenerator::generate_maze() {
    prepare();
    while (true) {
        create_right_walls();
        create_bottom_walls();
        if (current_line + 3 < size.y)
            next_line();
        else {
            complete_maze();
            break;
        }
    }
}

void EllerMazeGenerator::next_line() {
    for (uint i = 0; i < size.x; i++) {
        sf::Vector2u pos = {i, current_line + 1};
        if (field.get_cell(pos) == Cell::Wall)
            set_map[i] = numbers_of_sets++;
    }
    current_line += 2;
}

void EllerMazeGenerator::create_right_walls() {
    for (uint i = 0; i < size.x; i++) {
        sf::Vector2u pos = {i, current_line};
        sf::Vector2u under_pos = {i, current_line + 1};
        sf::Vector2u prev_pos = {i - 1, current_line};
        sf::Vector2u left_corner = {i - 1, current_line - 1};
        if (roll(WALL_PERCENT) and
            is_free_cell(pos) and
            is_free_cell(under_pos) and
            field.get_cell(prev_pos) != Cell::Wall and
            (current_line == 0 or field.get_cell(left_corner) != Cell::Wall)) {
            field.set_cell(pos, Cell::Wall);
            field.set_cell(under_pos, Cell::Wall);
        } else
            set_map[i + 1] = set_map[i];
    }
}

void EllerMazeGenerator::create_bottom_walls() {
    std::unordered_map<uint, uint> count_of_cell_without_bottom_wall;
    for (uint i = 0; i < size.x; i++)
        if (field.get_cell({i, current_line + 1}) != Cell::Wall)
            count_of_cell_without_bottom_wall[set_map[i]]++;
    for (uint i = 0; i < size.x; i++) {
        sf::Vector2u pos = {i, current_line + 1};
        if (roll(WALL_PERCENT) and
            field.get_cell(pos) != Cell::Wall and
            is_free_cell(pos) and
            count_of_cell_without_bottom_wall[set_map[i]] > 1) {
            count_of_cell_without_bottom_wall[set_map[i]]--;
            field.set_cell(pos, Cell::Wall);
        }
    }
}

void EllerMazeGenerator::complete_maze() {
    for (uint i = 0; i < size.x; i++) {
        sf::Vector2u pos = {i, current_line};
        sf::Vector2u under_pos = {i, current_line + 1};
        if (i < size.x and set_map[i] != set_map[i + 1])
            field.set_cell(pos, Cell::Empty);
        field.set_cell(under_pos, Cell::Wall);
    }
    field.set_start(field.get_start());
    field.set_finish(field.get_finish());
}

bool EllerMazeGenerator::is_free_cell(sf::Vector2u pos) const {
    return field.get_cell(pos) != Cell::Start and
           field.get_cell(pos) != Cell::Finish;
}
