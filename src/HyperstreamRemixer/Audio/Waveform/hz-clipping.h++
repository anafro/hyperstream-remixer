#pragma once
#include "HyperstreamRemixer/Math/factor-clipping.h++"


namespace HyperstreamRemixer::Audio::Waveform {
    using namespace Math;

    inline float clip_hz(const float hz) {
        return clip(0.f, hz, 20000.f);
    }
}
