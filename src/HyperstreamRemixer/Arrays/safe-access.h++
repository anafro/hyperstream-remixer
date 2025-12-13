#pragma once
#include <cstdint>

#include "boundaries.h++"

namespace HyperstreamRemixer::Arrays {
    template<typename T>
    [[nodiscard]]
    T safe_get(const T *array, const size_t index, const size_t array_length, const T by_default) {
        if (not index_is_safe(index, array_length)) {
            return by_default;
        }

        return array[index];
    }
}