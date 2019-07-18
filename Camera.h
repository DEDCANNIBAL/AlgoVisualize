#ifndef ALGOVISUALIZE_CAMERA_H
#define ALGOVISUALIZE_CAMERA_H


#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Camera {
    sf::View view;
    sf::RenderWindow &window;
    sf::Vector2u window_size;
    sf::Vector2f prev_mouse_pos;
    float zoom;
public:
    explicit Camera(sf::FloatRect field_of_view, sf::RenderWindow &window);

    void updateEvent(const sf::Event &event);

    void setView() { window.setView(view); };
};


#endif //ALGOVISUALIZE_CAMERA_H
