#pragma once
#include <HyperstreamRemixer/Concepts/primitive.h++>

namespace HyperstreamRemixer::Math {
using namespace Concepts;

template <numeric T>
auto clip(T min, T value, T max) -> T {
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}
} // namespace HyperstreamRemixer::Math
