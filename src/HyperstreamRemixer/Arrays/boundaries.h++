#pragma once

#include <cstddef>
namespace HyperstreamRemixer::Arrays {
inline auto index_is_safe(const std::size_t index, const std::size_t length) -> bool {
    return index < length;
}
} // namespace HyperstreamRemixer::Arrays
