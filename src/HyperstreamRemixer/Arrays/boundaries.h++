#pragma once

#include <cstddef>
namespace HyperstreamRemixer::Arrays {
inline bool index_is_safe(const std::size_t index, const std::size_t length) {
    return index < length;
}
} // namespace HyperstreamRemixer::Arrays
