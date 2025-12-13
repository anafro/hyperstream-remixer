#pragma once
#include <cmath>
#include "waveform-types.h++"

namespace HyperstreamRemixer::Sound::Waveform {
    inline wf_amplitude_t interpolate_catmull_rom(
        const wf_amplitude_t a,
        const wf_amplitude_t b,
        const wf_amplitude_t c,
        const wf_amplitude_t d,
        const double f
    ) {
        return std::ceil(.5 * ((2 * b) + (-a + c) * f + (2 * a - 5 * b + 4 * c - d) * f * f + (-a + 3 * b - 3 * c + d) * f * f * f));
    }
}
