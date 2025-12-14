#pragma once
#include <HyperstreamRemixer/Concepts/primitive.h++>

namespace HyperstreamRemixer::Sugar {
typedef double percent_t;
using namespace Concepts;

[[nodiscard]] consteval auto operator""_percent(const long double percents) -> percent_t {
    return percents / 100.0;
}

[[nodiscard]] consteval auto operator""_percent(const unsigned long long percents) -> percent_t {
    return percents / 100.0;
}
} // namespace HyperstreamRemixer::Sugar
