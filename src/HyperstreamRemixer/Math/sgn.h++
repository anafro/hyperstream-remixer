#pragma once
#include <HyperstreamRemixer/Concepts/primitive.h++>
#include <cstdint>

namespace HyperstreamRemixer::Math {
using namespace Concepts;

template <numeric T>
int32_t sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
} // namespace HyperstreamRemixer::Math
