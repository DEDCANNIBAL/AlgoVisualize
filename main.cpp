#include <thread>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "FieldDrawer.h"
#include "PathFinder.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "FieldInterface.h"


int main() {
    sf::Vector2u size(50, 35);
    Field field(size);
    FieldDrawer field_drawer(field);
    field.randomize();

    PathFinder path_finder(field);
    path_finder.set_delay(0.03);
    auto current_algorithm = &PathFinder::bfs;

    sf::RenderWindow window(sf::VideoMode(1000, 700), "AlgoVisualize");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    FieldInterface field_interface(field, window);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Algorithms");

        if (ImGui::RadioButton("BFS", true)) current_algorithm = &PathFinder::bfs;

        if (ImGui::Button("Start")){
            path_finder.stop();
            std::thread path_finding_thread(&PathFinder::bfs, &path_finder);
            path_finding_thread.detach();
        };

        if (ImGui::Button("Stop")){
            path_finder.stop();
        };

        ImGui::End();

        field_drawer.update();
        if (path_finder.is_finished())
            field_drawer.update(path_finder.get_path());

        window.clear(sf::Color::White);
        window.draw(field_drawer);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}