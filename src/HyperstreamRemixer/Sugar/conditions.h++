#pragma once
#include <HyperstreamRemixer/Concepts/primitive.h++>

namespace HyperstreamRemixer::Sugar {
    using namespace Concepts;

    template<numeric T>
    bool between(const T min, const T value, const T max) {
        return min <= value && value <= max;
    }
}