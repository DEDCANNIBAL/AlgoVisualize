#include <random>
#include <experimental/random>

#include "maze_generator.h"
#include "Field.h"
#include "PathFinderManager.h"
#include "AStar.h"

using namespace std::experimental;

const int GRID_SIZE = 50;
const int MIN_SIZE = 5;
const int MAX_SIZE = 30;
const float ROOM_RATE = 0.50;
const float CORRIDORS_LINEARITY = 0.90;

FieldGenerator::FieldGenerator(Field &field) :
        field(field),
        wall_field(field.get_size()) {
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
    for (int grid_x = 2; grid_x + GRID_SIZE < width - 1; grid_x += GRID_SIZE)
        for (int grid_y = 2; grid_y + GRID_SIZE < height - 1; grid_y += GRID_SIZE)
            if (roll(ROOM_RATE)) {
                create_room(grid_x, grid_y);
            }
}

void FieldGenerator::create_corridors() {
    for (int i = 0; i < rooms.size(); i++) {
        auto room1 = rooms[i];
        auto room2 = rooms[(i + randint(0ul, rooms.size() - 1)) % rooms.size()];
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

void FieldGenerator::create_room(int grid_x, int grid_y) {
    auto width = randint(MIN_SIZE, MAX_SIZE);
    auto height = randint(MIN_SIZE, MAX_SIZE);
    auto room_x = randint(grid_x, grid_x + GRID_SIZE - width);
    auto room_y = randint(grid_y, grid_y + GRID_SIZE - height);
    Room room({room_x, room_y}, {width, height});
    rooms.push_back(room);
    spawn_room(room);
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
    return rooms[randint(0ul, rooms.size() - 1)];
}

inline bool roll(float chance) {
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    static std::uniform_real_distribution distribution(0.f, 1.f);
    return distribution(gen) < chance;
}
