#ifndef ALGOVISUALIZE_UTILS_H
#define ALGOVISUALIZE_UTILS_H

#include <random>
#include <algorithm>

template <class T>
auto choice(T &contaner){
    static auto generator = std::mt19937{std::random_device{}()};
    static T samples_container;
    std::sample(contaner.begin(), contaner.end(),
                std::back_inserter(samples_container), 1, generator);
    return *--samples_container.end();
}

#endif //ALGOVISUALIZE_UTILS_H
