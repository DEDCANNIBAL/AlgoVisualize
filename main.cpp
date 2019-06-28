#include <thread>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "FieldDrawer.h"
#include "BreadthFirstSearch.h"
#include "PathFinderManager.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "FieldInterface.h"


int main() {
    sf::Vector2u size(50, 50);
    Field field(size);
    FieldDrawer field_drawer(field, 20);
    field.randomize();

    BreadthFirstSearch path_finder(field);
    PathFinderManager path_finder_manager;

    path_finder_manager.set_algorithm(dynamic_cast<PathFinder*>(&path_finder));
    path_finder_manager.set_delay(0.001);

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "AlgoVisualize");
    //window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    FieldInterface field_interface(field);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                path_finder_manager.stop();
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Algorithms");

        if (ImGui::RadioButton("BFS", true))
            path_finder_manager.set_algorithm(dynamic_cast<PathFinder*>(&path_finder));

        if (ImGui::Button("Start")){
            path_finder_manager.start_in_thread();
        };

        if (ImGui::Button("Continue")){
            path_finder_manager.proceed_in_thread();
        };

        if (ImGui::Button("Stop")){
            path_finder_manager.stop();
        };

        ImGui::End();

        field_drawer.update();
        if (path_finder.is_finished())
            field_drawer.update(path_finder.get_path());

        auto mouse_pos = sf::Mouse::getPosition(window);
        auto cell_pos = field_drawer.mouse_to_cell(mouse_pos);
        if (not path_finder_manager.is_working())
           field_interface.update(cell_pos);

        window.clear(sf::Color::White);
        window.draw(field_drawer);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}