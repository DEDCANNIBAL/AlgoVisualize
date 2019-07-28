#ifndef ALGOVISUALIZE_MAZE_GENERATOR_H
#define ALGOVISUALIZE_MAZE_GENERATOR_H

#include <SFML/Graphics/Rect.hpp>
#include "Field.h"

using Room = sf::IntRect;

class FieldGenerator{
    Field &field;
    Field wall_field;
    std::vector<Room> rooms;
public:
    FieldGenerator(Field &field);
    void prepare_field();
    void prepare_wall_field();
    void generate_maze();
    void create_rooms();
    void create_corridors();
    sf::Vector2i get_random_wall_of_room_not_adjacent_to_corridor(Room room);
    sf::Vector2i get_random_wall_of_room(Room room);
    void make_corridor_between(sf::Vector2i cell1, sf::Vector2i cell2);
    void create_room(int x, int y);
    void spawn_room(Room room);
    Room &get_random_room();
};

inline bool roll(float chance);

#endif //ALGOVISUALIZE_MAZE_GENERATOR_H
