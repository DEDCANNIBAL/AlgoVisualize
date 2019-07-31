#include "Camera.h"

const float PLUS_ZOOM = 1.25, MINUS_ZOOM = 0.80, VELOCITY = 600;

Camera::Camera(sf::FloatRect field_of_view, sf::RenderWindow &window) :
        view(field_of_view), window(window), window_size(window.getSize()), zoom(1) {}

void Camera::updateEvent(const sf::Event &event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) {
            view.zoom(PLUS_ZOOM);
            zoom *= PLUS_ZOOM;

        } else {
            view.zoom(MINUS_ZOOM);
            zoom *= MINUS_ZOOM;
        }
    }
    if (event.type == sf::Event::Resized) {
        auto center = view.getCenter();
        auto size = view.getSize();
        auto new_window_size = window.getSize();
        auto dx = static_cast<float > (new_window_size.x) / window_size.x,
                dy = static_cast<float > (new_window_size.y) / window_size.y;
        view.setCenter(center.x * dx, center.y * dy);
        view.setSize(size.x * dx, size.y * dy);
        window_size = new_window_size;
    }
    if (event.type == sf::Event::MouseMoved) {
        auto mouse_pos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
        auto delta_mouse_pos = mouse_pos - prev_mouse_pos;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            view.move(-delta_mouse_pos * zoom);
        prev_mouse_pos = mouse_pos;
    }
}
