#pragma once
#include <HyperstreamRemixer/Concepts/primitive.h++>

namespace HyperstreamRemixer::Sugar {
using namespace Concepts;

template <numeric T>
auto between(const T min, const T value, const T max) -> bool {
    return min <= value && value <= max;
}
} // namespace HyperstreamRemixer::Sugar
