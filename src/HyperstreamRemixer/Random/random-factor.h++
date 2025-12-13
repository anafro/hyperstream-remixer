#pragma once
#include <cstdlib>

[[nodiscard]]
inline double random_factor() {
    return (static_cast<double>(rand()) / (RAND_MAX)) + 1;
}
