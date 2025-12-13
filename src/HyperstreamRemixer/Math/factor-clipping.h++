#pragma once
#include "clipping.h++"

namespace HyperstreamRemixer::Math {
    inline double clip_factor(const double factor) {
        return clip(0.0, factor, 1.0);
    }
}
