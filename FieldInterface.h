#ifndef ALGOVISUALIZE_FIELDINTERFACE_H
#define ALGOVISUALIZE_FIELDINTERFACE_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

#include "Field.h"


namespace FieldInterfaceFSM {
    struct Transition {
        enum {
            NotPressed = 0,
            WallPressed,
            EmptyPressed,
            StartPressed,
            FinishPressed,
            Count
        };
    };

    struct State {
        enum {
            None = 0,
            WallSpawn,
            WallRemoving,
            StartMoving,
            FinishMoving,
            Count
        };
    };

    static std::vector<std::vector<std::pair<int, std::function<void(Field &, sf::Vector2u)>>>> fsm;

    void dummy(Field &, sf::Vector2u);

    void spawn_wall(Field &, sf::Vector2u);

    void remove_wall(Field &, sf::Vector2u);

    void move_start(Field &, sf::Vector2u);

    void move_finish(Field &, sf::Vector2u);

    void init_fsm();
};


class FieldInterface {

    Field &field;

    uint get_transition(sf::Vector2u cell_pos);

    int current_state;

public:
    explicit FieldInterface(Field &field);

    void update(sf::Vector2u cell_pos);
};


#endif //ALGOVISUALIZE_FIELDINTERFACE_H
