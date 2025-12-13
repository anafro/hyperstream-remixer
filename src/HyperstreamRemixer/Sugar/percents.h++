#pragma once
#include <HyperstreamRemixer/Concepts/primitive.h++>

namespace HyperstreamRemixer::Sugar {
    typedef double percent_t;
    using namespace Concepts;

    [[nodiscard]] consteval percent_t operator""_percent(const long double percents) {
        return percents / 100.0;
    }

    [[nodiscard]] consteval percent_t operator""_percent(const unsigned long long percents) {
        return percents / 100.0;
    }
}
