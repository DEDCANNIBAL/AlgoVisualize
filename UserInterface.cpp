#include <climits>
#include "UserInterface.h"


using namespace UserInterfaceFSM;

const uint NULL_STATE = UINT_MAX;


void UserInterfaceFSM::init_fsm() {
    fsm.resize(State::Count,
               std::vector<uint>(Transition::Count, NULL_STATE)
    );
    button_for_state.resize(State::Count);

    fsm[State::None][Transition::StartPressed] = State::PathFinding;

    fsm[State::PathFinding][Transition::PathFound] = State::PathFound;
    fsm[State::PathFinding][Transition::PausePressed] = State::Pause;
    fsm[State::PathFinding][Transition::ClearWallsPressed] = State::None;

    fsm[State::PathFound][Transition::ClearPathPressed] = State::None;
    fsm[State::PathFound][Transition::ClearWallsPressed] = State::None;
    fsm[State::PathFound][Transition::StartPressed] = State::PathFinding;

    fsm[State::Pause][Transition::ContinuePressed] = State::PathFinding;
    fsm[State::Pause][Transition::ClearWallsPressed] = State::None;
    fsm[State::Pause][Transition::ClearPathPressed] = State::None;

    button_for_state[State::None] = {"Start", "Randomize", "Clear Walls", "Go to Start", "Go to Finish"};
    button_for_state[State::PathFinding] = {"Restart", "Pause", "Clear Walls", "Go to Start", "Go to Finish"};
    button_for_state[State::PathFound] = {"Restart", "Clear Path", "Clear Walls",
                                          "Go to Start", "Go to Finish"};
    button_for_state[State::Pause] = {"Continue", "Cancel", "Clear Walls",
                                      "Go to Start", "Go to Finish"};

    transition_for_action["Start"] = Transition::StartPressed;
    transition_for_action["Clear Walls"] = Transition::ClearWallsPressed;
    transition_for_action["Restart"] = Transition::StartPressed;
    transition_for_action["Pause"] = Transition::PausePressed;
    transition_for_action["Clear Path"] = Transition::ClearPathPressed;
    transition_for_action["Continue"] = Transition::ContinuePressed;
    transition_for_action["Cancel"] = Transition::ClearPathPressed;
}

void UserInterface::algorithms_interface() {
    static int current_algorithm = 0;
    ImGui::Begin("Algorithms");
    for (int i = 0; i < list_of_algorithms.size(); i++)
        if (ImGui::RadioButton(list_of_algorithms[i].c_str(), i == current_algorithm)) {
            actions[list_of_algorithms[i]]();
            current_algorithm = i;
        }
    ImGui::End();
}

void UserInterface::update(bool is_path_found) {
    ImGui::SFML::Update(window, delta_clock.restart());
    if (is_path_found or current_state == State::None)
        algorithms_interface();
    auto transition = get_transition(is_path_found);
    auto new_state = fsm[current_state][transition];
    if (new_state == NULL_STATE)
        return;
    current_state = new_state;
}

uint UserInterface::get_transition(bool is_path_found) {
    ImGui::Begin("Control Panel");
    uint transition = Transition::NotPressed;
    for (auto action_name:button_for_state[current_state])
        if (ImGui::Button(action_name.c_str())) {
            actions[action_name]();
            transition = transition_for_action[action_name];
        }
    ImGui::End();
    if (is_path_found and current_state == State::PathFinding)
        transition = Transition::PathFound;
    return transition;
}

UserInterface::UserInterface(sf::RenderWindow &window) :
        window(window), current_state(State::None) {
    init_fsm();
    list_of_algorithms = {"BFS", "A*"};
}
