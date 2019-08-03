#include <random>

#include "MazeGenerator.h"

const float WALL_PERCENT = 0.45;


MazeGenerator::MazeGenerator(Field &field) : field(field) {

}

void MazeGenerator::prepare() {
    for (uint y = 0; y < field.get_size().y; y++)
        for (uint x = 0; x < field.get_size().x; x++)
            field.set_cell({x, y}, Cell::Empty);
}

void MazeGenerator::generate_maze() {
    prepare();
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    auto size = field.get_size();
    static std::uniform_int_distribution<> distribution_x(0, size.x - 1);
    static std::uniform_int_distribution<> distribution_y(0, size.y - 1);
    for (int i = 0; i < size.x * size.y * WALL_PERCENT; i++) {
        auto x = gen() % size.x;
        auto y = gen() % size.y;
        sf::Vector2u pos(x, y);
        if (pos != field.get_start() && pos != field.get_finish())
            field.set_cell(pos, Cell::Wall);
    }
    field.set_start(field.get_start());
    field.set_finish(field.get_finish());
}
