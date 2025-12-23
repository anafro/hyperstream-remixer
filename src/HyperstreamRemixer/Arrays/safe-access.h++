#pragma once
#include "boundaries.h++"
#include <cstddef>

namespace HyperstreamRemixer::Arrays {
template <typename T>
[[nodiscard]]
auto safe_get(const T *array, const std::size_t index, const std::size_t array_length, const T by_default) -> T {
    if (not index_is_safe(index, array_length)) {
        return by_default;
    }

    return array[index];
}

template <typename T>
void safe_set(T *array, const std::size_t index, const std::size_t array_length, const T &value) {
    if (not index_is_safe(index, array_length)) {
        return;
    }

    array[index] = value;
}
} // namespace HyperstreamRemixer::Arrays
