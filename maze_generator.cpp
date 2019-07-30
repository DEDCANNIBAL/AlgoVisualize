#include <random>
#include <experimental/random>

#include "maze_generator.h"
#include "Field.h"
#include "PathFinderManager.h"
#include "AStar.h"
#include "BreadthFirstSearch.h"
#include "utils.h"

using namespace std::experimental;

const int GRID_SIZE = 50;
const int MIN_SIZE = 5;
const int MAX_SIZE = 30;
const float ROOM_RATE = 0.80;
const float CORRIDORS_LINEARITY = 0.90;

FieldGenerator::FieldGenerator(Field &field) :
        field(field),
        wall_field(field.get_size()),
        grid_width((field.get_size().x - 2) / GRID_SIZE),
        grid_height((field.get_size().y - 2) / GRID_SIZE) {
    prepare_field();
    prepare_wall_field();
}

void FieldGenerator::prepare_field() {
    for (uint y = 0; y < field.get_size().y; y++)
        for (uint x = 0; x < field.get_size().x; x++)
            field.set_cell({x, y}, Cell::Wall);
}

void FieldGenerator::prepare_wall_field() {
    auto[width, height] = field.get_size();
    auto walls_number = width * height * (1.f - CORRIDORS_LINEARITY);
    for (uint i = 0, x, y; i < walls_number; i++) {
        x = randint(1u, width - 1);
        y = randint(1u, height - 1);
        wall_field.set_cell({x, y}, Cell::Wall);
    }
    for (uint x = 0; x < width; x++) {
        wall_field.set_cell({x, 0}, Cell::Wall);
        wall_field.set_cell({x, height - 1}, Cell::Wall);
    }
    for (uint y = 0; y < height; y++) {
        wall_field.set_cell({0, y}, Cell::Wall);
        wall_field.set_cell({width - 1, y}, Cell::Wall);
    }
}

void FieldGenerator::generate_maze() {
    create_rooms();
    create_corridors();
    field.set_start(field.get_start());
    field.set_finish(field.get_finish());
}

void FieldGenerator::create_rooms() {
    auto[width, height] = field.get_size();
    rooms_grid.resize(width, std::vector<std::optional<Room >>(height));
    for (int grid_x = 2, grid_i = 0; grid_i < grid_width; grid_x += GRID_SIZE, grid_i++)
        for (int grid_y = 2, grid_j = 0; grid_j < grid_height; grid_y += GRID_SIZE, grid_j++)
            if (roll(ROOM_RATE)) {
                rooms_grid[grid_i][grid_j] = create_room(grid_x, grid_y);
            }
}

void FieldGenerator::create_corridors() {
    for (int i = 0; i < grid_width; i++)
        for (int j = 0; j < grid_height; j++){
            if (not rooms_grid[i][j].has_value()) continue;
            auto room1 = *rooms_grid[i][j];
            auto room2 = get_random_adjacent_room(i, j);
            make_corridor_between(
                    get_random_wall_of_room_not_adjacent_to_corridor(room1),
                    get_random_wall_of_room_not_adjacent_to_corridor(room2)
            );
        }
    make_corridor_between(
            get_random_wall_of_room_not_adjacent_to_corridor(get_random_room()),
            static_cast<sf::Vector2i>(field.get_start())
    );
    make_corridor_between(
            get_random_wall_of_room_not_adjacent_to_corridor(get_random_room()),
            static_cast<sf::Vector2i>(field.get_finish())
    );
}

Room FieldGenerator::create_room(int grid_x, int grid_y) {
    auto width = randint(MIN_SIZE, MAX_SIZE);
    auto height = randint(MIN_SIZE, MAX_SIZE);
    auto room_x = randint(grid_x, grid_x + GRID_SIZE - width);
    auto room_y = randint(grid_y, grid_y + GRID_SIZE - height);
    Room room({room_x, room_y}, {width, height});
    spawn_room(room);
    return room;
}

void FieldGenerator::spawn_room(Room room) {
    for (uint x = room.left; x < room.left + room.width; x++)
        for (uint y = room.top; y < room.top + room.height; y++)
            field.set_cell({x, y}, Cell::Empty);
    for (uint x = room.left - 1; x <= room.left + room.width; x++)
        for (uint y = room.top - 1; y <= room.top + room.height; y++)
            wall_field.set_cell({x, y}, Cell::Wall);
}

sf::Vector2i FieldGenerator::get_random_wall_of_room_not_adjacent_to_corridor(Room room) {
    static const std::vector<sf::Vector2i> shifts = {{0,  +1},
                                                     {0,  -1},
                                                     {+1, 0},
                                                     {-1, 0}};
    sf::Vector2i wall;
    while (true) {
        wall = get_random_wall_of_room(room);
        auto wall_number = 0;
        for (auto shift: shifts)
            wall_number +=
                    field.get_cell(static_cast<sf::Vector2u>(wall + shift)) == Cell::Wall;
        if (wall_number == 3)
            return wall;
    }
}

sf::Vector2i FieldGenerator::get_random_wall_of_room(Room room) {
    if (roll(0.5))
        return {
                room.left + randint(0, room.width - 1),
                room.top - 1 + roll(0.5) * (room.height + 1)
        };
    else
        return {
                room.left - 1 + roll(0.5) * (room.width + 1),
                room.top + randint(0, room.height - 1)
        };
}

void FieldGenerator::make_corridor_between(sf::Vector2i cell1, sf::Vector2i cell2) {
    PathFinderManager manager(wall_field);
    manager.set_algorithm<AStar>();
    wall_field.set_start(static_cast<sf::Vector2u>(cell1));
    wall_field.set_finish(static_cast<sf::Vector2u>(cell2));
    wall_field.set_cell(static_cast<sf::Vector2u>(cell1), Cell::Empty);
    wall_field.set_cell(static_cast<sf::Vector2u>(cell2), Cell::Empty);
    auto &path = manager.find_and_return_path();
    for (auto cell: path)
        field.set_cell(static_cast<sf::Vector2u>(cell), Cell::Empty);
}

Room &FieldGenerator::get_random_room() {
    while (true) {
        auto room = rooms_grid[randint(0ul, grid_width - 1)][randint(0ul, grid_height - 1)];
        if (room.has_value())
            return *room;
    }
}

Room FieldGenerator::get_random_adjacent_room(size_t grid_i, size_t grid_j) {
    static const std::vector<std::pair<int, int>> shifts = {{-1, -1}, {-1, 0}, {-1, 1},
                                                            {0,  -1},          {0,  1},
                                                            {1,  -1}, {1, 0},  {1, 1}};
    std::vector<Room> adjacent_rooms;
    for (auto [shift_i, shift_j]: shifts){
        auto i = grid_i + shift_i, j = grid_j + shift_j;
        if (i < grid_width and j < grid_height and rooms_grid[i][j].has_value())
            adjacent_rooms.push_back(*rooms_grid[i][j]);
    }
    return choice(adjacent_rooms);
}

inline bool roll(float chance) {
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    static std::uniform_real_distribution distribution(0.f, 1.f);
    return distribution(gen) < chance;
}
