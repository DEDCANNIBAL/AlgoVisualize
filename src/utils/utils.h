#ifndef ALGOVISUALIZE_UTILS_H
#define ALGOVISUALIZE_UTILS_H

#include <experimental/random>

template <class T>
auto choice(T &contaner){
    auto index = std::experimental::randint(0ul, contaner.size() - 1);
    for(auto element:contaner)
        if(not index--)
            return element;
}

#endif //ALGOVISUALIZE_UTILS_H
