#ifndef ALGOVISUALIZE_FIELD_H
#define ALGOVISUALIZE_FIELD_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "SafeVector.h"


bool operator<(const sf::Vector2u &a, const sf::Vector2u &b);

struct Cell {
    enum {
        Empty = 0,
        Wall,
        Start,
        Finish,
    };
};

class Field {
    sf::Vector2u size;
    std::vector<std::vector<u_int8_t>> obstacles;
    std::vector<std::vector<bool>> visited;
    sf::Vector2u start, finish;
    SafeCellsVector changed_cells;
    void change_cell(sf::Vector2u);
    void change_all_cells();

public:
    explicit Field(sf::Vector2u size);

    sf::Vector2u get_size() const { return size; }

    void set_cell(sf::Vector2u pos, u_int8_t cell);

    u_int8_t get_cell(sf::Vector2u pos) const;

    u_int8_t visit_cell(sf::Vector2u pos);

    void set_start(sf::Vector2u start);

    void set_finish(sf::Vector2u finish);

    sf::Vector2u get_start() const { return start; }

    sf::Vector2u get_finish() const { return finish; }

    bool is_visited(sf::Vector2u pos) const { return visited[pos.x][pos.y]; }

    void reset_visited();

    void clear_obstacles();

    void randomize();

    std::vector<sf::Vector2u> observe_changed_cells();
};


#endif //ALGOVISUALIZE_FIELD_H
