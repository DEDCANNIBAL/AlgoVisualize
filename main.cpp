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
#include "Camera.h"


int main() {
    sf::Vector2u size(1000, 1000);
    Field field(size);
    auto cell_size = 20;
    FieldDrawer field_drawer(field, cell_size);
    field.randomize();

    BreadthFirstSearch path_finder(field);
    PathFinderManager path_finder_manager;
    path_finder_manager.set_algorithm(dynamic_cast<PathFinder *>(&path_finder));
    path_finder_manager.set_delay(0.0001);

    sf::Vector2f window_size(1000, 1000);
    auto window_width_in_cells = window_size.x / cell_size - 1;
    field.set_start(size / 2u - sf::Vector2u(window_width_in_cells / 2, 0));
    field.set_finish(size / 2u + sf::Vector2u(window_width_in_cells / 2, 0));
    auto field_size = static_cast<float>(cell_size) * static_cast<sf::Vector2f>(size);
    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "AlgoVisualize");
    Camera camera(sf::FloatRect((field_size - window_size) / 2.f, window_size), window);
    ImGui::SFML::Init(window);
    FieldInterface field_interface(field);

    sf::Clock delta_clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                path_finder_manager.stop();
                window.close();
            }
            camera.updateEvent(event);
        }

        ImGui::SFML::Update(window, delta_clock.restart());

        ImGui::Begin("Algorithms");

        if (ImGui::RadioButton("BFS", true))
            path_finder_manager.set_algorithm(dynamic_cast<PathFinder *>(&path_finder));

        if (ImGui::Button("Start")) {
            path_finder_manager.start_in_thread();
        };

        if (ImGui::Button("Continue")) {
            path_finder_manager.proceed_in_thread();
        };

        if (ImGui::Button("Stop")) {
            path_finder_manager.stop();
        };

        ImGui::End();

        field_drawer.update();
        if (path_finder.is_finished())
            field_drawer.update(path_finder.get_path());

        auto pixel_pos = sf::Mouse::getPosition(window);
        auto world_pos = window.mapPixelToCoords(pixel_pos);
        auto mouse_pos = sf::Vector2i(static_cast<int> (world_pos.x), static_cast<int> (world_pos.y));
        auto cell_pos = field_drawer.mouse_to_cell(mouse_pos);
        if (not path_finder_manager.is_working())
            field_interface.update(cell_pos);

        camera.setView();
        window.clear(sf::Color::White);
        window.draw(field_drawer);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}