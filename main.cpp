#include <thread>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "FieldDrawer.h"
#include "PathFinder.h"
#include "imgui.h"
#include "imgui-SFML.h"


int main() {
    sf::Vector2u size(50, 35);
    Field field(size);
    FieldDrawer field_drawer(field);
    field.randomize();

    PathFinder path_finder(field);
    path_finder.set_delay(0.03);
    std::thread path_finding_thread(&PathFinder::bfs, &path_finder);
    path_finding_thread.detach();

    sf::RenderWindow window(sf::VideoMode(1000, 700), "AlgoVisualize");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

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