#ifndef ALGOVISUALIZE_USERINTERFACE_H
#define ALGOVISUALIZE_USERINTERFACE_H

#include <functional>
#include <variant>
#include <unordered_map>

#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui.h>
#include <imgui-SFML.h>


namespace UserInterfaceFSM {
    struct Transition {
        enum {
            NotPressed = 0,
            StartPressed,
            PathFound,
            PausePressed,
            ContinuePressed,
            ClearPathPressed,
            ClearWallsPressed,
            Count
        };
    };

    struct State {
        enum {
            None = 0,
            PathFinding,
            Pause,
            PathFound,
            Count
        };
    };
    static std::vector<std::vector<uint>> fsm;
    static std::vector<std::vector<std::variant<std::string,
            std::pair<std::string, std::vector<std::string>>>>> button_for_state;
    static std::unordered_map<std::string, uint> transition_for_action;

    void init_fsm();
};


class UserInterface {
    sf::RenderWindow &window;
    std::unordered_map<std::string, std::function<void()>> actions;
    std::vector<std::string> list_of_algorithms;
    uint current_state;
    sf::Clock delta_clock;
    float delay;
    int cells_visited, path_length;

    uint get_transition(bool is_path_found);

    void algorithms_interface();

public:
    explicit UserInterface(sf::RenderWindow &window);

    void add_action(const std::string &action_name, std::function<void()> action) { actions[action_name] = action; }

    void update(bool is_path_found);

    void render() { ImGui::SFML::Render(window); }

    float getDelay() const;

    void set_cells_visited(int cellsVisited);

    void set_path_length(int pathLength);

    bool make_button(std::string &action_name);
};


#endif //ALGOVISUALIZE_USERINTERFACE_H
