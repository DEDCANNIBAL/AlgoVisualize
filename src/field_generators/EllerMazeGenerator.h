#ifndef ALGOVISUALIZE_ELLERMAZEGENERATOR_H
#define ALGOVISUALIZE_ELLERMAZEGENERATOR_H


#include "MazeGenerator.h"


class EllerMazeGenerator : public MazeGenerator {
    sf::Vector2u size;
    uint numbers_of_sets;
    uint current_line;
    std::vector<uint> set_map;

    void prepare() override;

    void create_right_walls();

    void create_bottom_walls();

    void complete_maze();

    void next_line();

    bool is_free_cell(sf::Vector2u pos) const;

public:
    explicit EllerMazeGenerator(Field &field);

    void generate_maze() override;

};


#endif //ALGOVISUALIZE_ELLERMAZEGENERATOR_H
