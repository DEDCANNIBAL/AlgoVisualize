#ifndef ALGOVISUALIZE_FIELDINTERFACE_H
#define ALGOVISUALIZE_FIELDINTERFACE_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

#include "Field.h"


class FieldInterface {
    Field &field;
public:
    FieldInterface(Field &field, sf::Window &window);
    void update(sf::Event event);
};


#endif //ALGOVISUALIZE_FIELDINTERFACE_H
