#pragma once
#include <HyperstreamRemixer/Concepts/primitive.h++>
#include <cstdint>

namespace HyperstreamRemixer::Math {
using namespace Concepts;

template <numeric T>
auto sgn(T val) -> int32_t {
    return (T(0) < val) - (val < T(0));
}
} // namespace HyperstreamRemixer::Math
