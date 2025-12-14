#pragma once
#include "clipping.h++"

namespace HyperstreamRemixer::Math {
inline auto clip_factor(const double factor) -> double {
    return clip(0.0, factor, 1.0);
}
} // namespace HyperstreamRemixer::Math
