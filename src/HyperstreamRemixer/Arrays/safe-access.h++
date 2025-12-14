#pragma once
#include "boundaries.h++"

namespace HyperstreamRemixer::Arrays {
template <typename T>
[[nodiscard]]
auto safe_get(const T *array, const std::size_t index, const std::size_t array_length, const T by_default) -> T {
    if (not index_is_safe(index, array_length)) {
        return by_default;
    }

    return array[index];
}
} // namespace HyperstreamRemixer::Arrays
