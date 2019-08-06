#ifndef ALGOVISUALIZE_SAFEVECTOR_H
#define ALGOVISUALIZE_SAFEVECTOR_H

#include <vector>
#include <mutex>

#include <SFML/System/Vector2.hpp>


class SafeCellsVector {
    std::vector<sf::Vector2u> cells;
    std::mutex cells_mutex;
public:
    void clear();
    void push_back(sf::Vector2u cell);
    std::vector<sf::Vector2u> get_vector();
    std::vector<sf::Vector2u> pop_vector();
};


#endif //ALGOVISUALIZE_SAFEVECTOR_H
