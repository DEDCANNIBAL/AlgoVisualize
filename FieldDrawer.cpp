#include "FieldDrawer.h"


void FieldDrawer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    for (auto &row:quads)
        target.draw(row, states);
    target.draw(boards);
}

void FieldDrawer::update() {
    for (uint i = 0; i < size.x; i++)
        for (uint j = 0; j < size.y; j++)
            set_color({i, j}, find_out_color({i, j}));
}

void FieldDrawer::update(const std::vector<sf::Vector2u> &path) {
    for (int i = 1; i < path.size() - 1; i++)
        set_color(path[i], sf::Color::Cyan);
}

sf::Color FieldDrawer::find_out_color(sf::Vector2u pos) const {
    if (field.get_cell(pos) == Cell::Wall)
        return sf::Color::Black;
    if (pos == field.get_start())
        return sf::Color::Green;
    if (pos == field.get_finish())
        return sf::Color::Red;
    return field.is_visited(pos) ? sf::Color::Blue : sf::Color::White;
}

FieldDrawer::FieldDrawer(Field &field, float cell_size) :
        size(field.get_size()),
        quads(field.get_size().x, sf::VertexArray(sf::Quads, 4 * size.y)),
        boards(sf::Lines, (size.x + size.y + 2) * 2),
        field(field),
        cell_size(cell_size) {
    initialize_boards();
    initialize_quads();
}

void FieldDrawer::initialize_board_pos(uint i, bool is_horizontal) {
    boards[i].position = (is_horizontal ?
                          sf::Vector2f(i / 2 - size.x - 1, 0) :
                          sf::Vector2f(0, i / 2)) * cell_size;
    boards[i + 1].position = (is_horizontal ?
                              sf::Vector2f(i / 2 - size.x - 1, size.y) :
                              sf::Vector2f(size.x, i / 2)) * cell_size;
}

void FieldDrawer::initialize_quads() {
    for (uint i = 0; i < size.x; i++)
        for (uint j = 0; j < size.y; j++) {
            set_position({i, j});
            set_color({i, j}, sf::Color::White);
        }
}

void FieldDrawer::initialize_boards() {
    for (uint i = 0; i < (size.x + size.y + 2) * 2; i += 2)
        initialize_board_pos(i, i > size.x * 2);
    for (uint i = 0; i < boards.getVertexCount(); i++)
        boards[i].color = sf::Color::Black;
}

void FieldDrawer::set_position(sf::Vector2u pos) {
    auto &row = quads[pos.x];
    const static sf::Vector2u shifts[] = {{0, 0},
                                          {0, 1},
                                          {1, 1},
                                          {1, 0}};
    for (int i = 0; i < 4; i++)
        row[pos.y * 4 + i].position = sf::Vector2f(pos + shifts[i]) * cell_size;
}

void FieldDrawer::set_color(sf::Vector2u pos, sf::Color col) {
    auto &row = quads[pos.x];
    for (int i = 0; i < 4; i++)
        row[pos.y * 4 + i].color = col;
}
