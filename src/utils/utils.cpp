#include "utils.h"



bool roll(float chance) {
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    static std::uniform_real_distribution distribution(0.f, 1.f);
    return distribution(gen) < chance;
}
