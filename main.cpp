#include <thread>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "FieldDrawer.h"
#include "BreadthFirstSearch.h"
#include "PathFinderManager.h"
#include "FieldInterface.h"
#include "Camera.h"
#include "UserInterface.h"


int main() {
    sf::Vector2u size(1000, 1000);
    Field field(size);
    auto cell_size = 20;
    FieldDrawer field_drawer(field, cell_size);

    BreadthFirstSearch path_finder(field);
    PathFinderManager path_finder_manager;
    path_finder_manager.set_algorithm(dynamic_cast<PathFinder *>(&path_finder));
    path_finder_manager.set_delay(0.0001);

    sf::Vector2f window_size(1000, 1000);
    auto window_width_in_cells = window_size.x / cell_size - 1;
    field.set_start(size / 2u - sf::Vector2u(window_width_in_cells / 2 + 1, 0));
    field.set_finish(size / 2u + sf::Vector2u(window_width_in_cells / 2, 0));
    auto field_size = static_cast<float>(cell_size) * static_cast<sf::Vector2f>(size);
    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "AlgoVisualize");
    Camera camera(sf::FloatRect((field_size - window_size) / 2.f, window_size), window);
    ImGui::SFML::Init(window);
    FieldInterface field_interface(field);
    UserInterface user_interface(window);

    user_interface.add_action("BFS", [&path_finder_manager, &path_finder]() {
        path_finder_manager.set_algorithm(dynamic_cast<PathFinder *>(&path_finder));
    });
    user_interface.add_action("Start", [&path_finder_manager]() {
        path_finder_manager.stop();
        path_finder_manager.start_in_thread();
    });
    user_interface.add_action("Randomize", [&path_finder, &field]() {
        field.clear_obstacles();
        path_finder.prepare();
        field.randomize();
    });
    user_interface.add_action("Clear Walls", [&path_finder_manager, &path_finder, &field]() {
        path_finder_manager.stop();
        field.clear_obstacles();
        path_finder.prepare();
    });
    user_interface.add_action("Restart", [&path_finder_manager]() {
        path_finder_manager.stop();
        path_finder_manager.start_in_thread();
    });
    user_interface.add_action("Pause", [&path_finder_manager]() {
        path_finder_manager.stop();
    });
    user_interface.add_action("Clear Path", [&path_finder]() {
        path_finder.prepare();
    });
    user_interface.add_action("Continue", [&path_finder_manager]() {
        path_finder_manager.proceed_in_thread();
    });
    user_interface.add_action("Cancel", [&path_finder]() {
        path_finder.prepare();
    });
    user_interface.add_action("Go to Start", [&camera]() {

    });
    user_interface.add_action("Go to Finish", [&camera]() {

    });

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

        auto pixel_pos = sf::Mouse::getPosition(window);
        auto world_pos = window.mapPixelToCoords(pixel_pos);
        auto mouse_pos = sf::Vector2i(static_cast<int> (world_pos.x), static_cast<int> (world_pos.y));
        auto cell_pos = field_drawer.mouse_to_cell(mouse_pos);
        user_interface.update(path_finder.is_finished());
        field_drawer.update();
        if (path_finder.is_finished())
            field_drawer.update(path_finder.get_path());
        if (not path_finder_manager.is_working())
            field_interface.update(cell_pos);

        camera.setView();
        window.clear(sf::Color::White);
        window.draw(field_drawer);
        user_interface.render();
        window.display();
    }

    ImGui::SFML::Shutdown();
}