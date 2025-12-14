#pragma once
#include <cstdlib>

[[nodiscard]]
inline auto random_factor() -> double {
    return (static_cast<double>(rand()) / (RAND_MAX)) + 1;
}
