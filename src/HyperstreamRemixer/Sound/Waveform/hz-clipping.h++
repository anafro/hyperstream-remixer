#pragma once
#include "HyperstreamRemixer/Math/factor-clipping.h++"

namespace HyperstreamRemixer::Sound::Waveform {
using namespace Math;

inline auto clip_hz(const float hz) -> float {
    return clip(0.f, hz, 20000.f);
}
} // namespace HyperstreamRemixer::Sound::Waveform
