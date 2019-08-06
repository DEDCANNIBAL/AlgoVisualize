#ifndef ALGOVISUALIZE_ROOMMAZEGENERATOR_H
#define ALGOVISUALIZE_ROOMMAZEGENERATOR_H


#include <SFML/Graphics/Rect.hpp>
#include <optional>

#include "MazeGenerator.h"

using Room = sf::IntRect;

class RoomMazeGenerator : public MazeGenerator {
    Field wall_field;
    std::vector<std::vector<std::optional<Room>>> rooms_grid;
    size_t grid_width, grid_height;

    void prepare() override;

    void prepare_field();

    void prepare_wall_field();

    void create_rooms();

    void create_corridors();

    sf::Vector2i get_random_wall_of_room_not_adjacent_to_corridor(Room room);

    sf::Vector2i get_random_wall_of_room(Room room);

    void make_corridor_between(sf::Vector2i cell1, sf::Vector2i cell2);

    Room create_room(int grid_x, int grid_y);

    void spawn_room(Room room);

    Room &get_random_room();

    Room get_random_adjacent_room(size_t grid_i, size_t grid_j);

public:
    explicit RoomMazeGenerator(Field &field);

    void generate_maze() override;

};


#endif //ALGOVISUALIZE_ROOMMAZEGENERATOR_H
