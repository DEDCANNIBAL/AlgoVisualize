#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "FieldDrawer.h"
#include "PathFinder.h"

int main() {
    sf::Vector2u size(20, 20);
    Field field(size);
    FieldDrawer field_drawer(field);
    PathFinder path_finder(field);
    field.randomize();
    path_finder.bfs();

    sf::RenderWindow window(sf::VideoMode(640, 480), "AlgoVisualize");
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
        field_drawer.update(path_finder.get_path());

        window.clear(sf::Color::White);
        window.draw(field_drawer);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}