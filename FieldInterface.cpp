#include <unordered_map>

#include "FieldInterface.h"


using namespace FieldInterfaceFSM;


auto FieldInterfaceFSM::init_fsm() {
    auto fsm = std::vector<std::vector<std::pair<int, std::function<void(Field &, sf::Vector2u)>>>>(
            State::Count,
            std::vector<std::pair<int, std::function<void(Field &, sf::Vector2u)>>>(Transition::Count)
    );

    fsm[State::None][Transition::WallPressed] = {State::WallRemoving, &dummy};
    fsm[State::None][Transition::EmptyPressed] = {State::WallSpawn, &dummy};
    fsm[State::None][Transition::StartPressed] = {State::StartMoving, &dummy};
    fsm[State::None][Transition::FinishPressed] = {State::FinishMoving, &dummy};

    fsm[State::WallRemoving][Transition::WallPressed] = {State::WallRemoving, &remove_wall};
    fsm[State::WallRemoving][Transition::NotPressed] = {State::None, &dummy};

    fsm[State::WallSpawn][Transition::EmptyPressed] = {State::WallSpawn, &spawn_wall};
    fsm[State::WallSpawn][Transition::NotPressed] = {State::None, &dummy};

    fsm[State::StartMoving][Transition::EmptyPressed] = {State::StartMoving, &move_start};
    fsm[State::FinishMoving][Transition::EmptyPressed] = {State::FinishMoving, &move_finish};

    fsm[State::StartMoving][Transition::NotPressed] = {State::None, &dummy};
    fsm[State::FinishMoving][Transition::NotPressed] = {State::None, &dummy};

    return fsm;
}

void FieldInterfaceFSM::dummy(Field &, sf::Vector2u) {};

void FieldInterfaceFSM::spawn_wall(Field &field, sf::Vector2u cell_pos) {
    field.set_cell(cell_pos, Cell::Wall);
};

void FieldInterfaceFSM::remove_wall(Field &field, sf::Vector2u cell_pos) {
    field.set_cell(cell_pos, Cell::Empty);
};

void FieldInterfaceFSM::move_start(Field &field, sf::Vector2u cell_pos) {
    field.set_start(cell_pos);
};

void FieldInterfaceFSM::move_finish(Field &field, sf::Vector2u cell_pos) {
    field.set_finish(cell_pos);
};

FieldInterface::FieldInterface(Field &field) :
        field(field), fsm(init_fsm()), cur_state(State::None) {
}

void FieldInterface::update(sf::Vector2u cell_pos) {
    auto transition = get_transition(cell_pos);
    auto [new_state, f] = fsm[cur_state][transition];
    if (f == nullptr) return;
    f(field, cell_pos);
    cur_state = new_state;
}


uint FieldInterface::get_transition(sf::Vector2u cell_pos) {
    if (not sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        return Transition::NotPressed;
    }
    auto cell = field.get_cell(cell_pos);
    static std::unordered_map<uint8_t, uint> cell_to_transition = {
            {Cell::Wall,   Transition::WallPressed},
            {Cell::Empty,  Transition::EmptyPressed},
            {Cell::Start,  Transition::StartPressed},
            {Cell::Finish, Transition::FinishPressed},
    };
    return cell_to_transition.at(cell);
}

