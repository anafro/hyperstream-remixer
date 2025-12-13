#pragma once
#include <cstdint>


namespace HyperstreamRemixer::Arrays {
    inline bool index_is_safe(const size_t index, const size_t length) {
        return index < length;
    }
}

