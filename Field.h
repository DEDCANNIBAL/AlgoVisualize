#ifndef ALGOVISUALIZE_FIELD_H
#define ALGOVISUALIZE_FIELD_H

#include <vector>
#include <SFML/System/Vector2.hpp>


bool operator<(const sf::Vector2u &a, const sf::Vector2u &b);

namespace Cell {
    enum {
        Empty = 0,
        Wall
    };
}

class Field {
    sf::Vector2u size;
    std::vector<std::vector<bool>> obstacles, visited;
    sf::Vector2u start, finish;
    std::vector<sf::Vector2u> changed_cells;
    void change_cell(sf::Vector2u);
    void change_all_cells();

public:
    explicit Field(sf::Vector2u size);

    sf::Vector2u get_size() const { return size; }

    void set_cell(sf::Vector2u pos, bool cell);

    bool get_cell(sf::Vector2u pos) const;

    bool visit_cell(sf::Vector2u pos);

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
