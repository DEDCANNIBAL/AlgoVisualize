#ifndef ALGOVISUALIZE_FIELDDRAWER_H
#define ALGOVISUALIZE_FIELDDRAWER_H

#include <SFML/Graphics.hpp>
#include "../Field.h"

class FieldDrawer : public sf::Drawable, public sf::Transformable {
    sf::Vector2u size;
    std::vector<sf::VertexArray> quads;
    sf::VertexArray vertex_path;
    size_t path_size;
    sf::VertexArray boards;
    Field &field;
    float cell_size;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void set_position(sf::Vector2u pos, float shift=0);

    void initialize_boards();

    void initialize_quads();

    void initialize_board_pos(uint i, bool is_horizontal);

    sf::Color find_out_color(sf::Vector2u pos) const;

    void start_animation(sf::Vector2u pos);

public:
    explicit FieldDrawer(Field &field, float cell_size = 20);

    void set_color(sf::Vector2u pos, sf::Color col);

    void update();

    void update(const std::vector<sf::Vector2u> &path);

    sf::Vector2u mouse_to_cell(sf::Vector2i);

};


#endif //ALGOVISUALIZE_FIELDDRAWER_H
