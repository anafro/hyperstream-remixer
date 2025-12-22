#pragma once
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"
#include <cstdlib>

namespace HyperstreamRemixer::Debug {
using namespace Sound::Waveform;

inline auto add_white_noise_if(const wf_amplitude_t original_amplitude, const bool should_add_signal) -> wf_amplitude_t {
    if (should_add_signal) {
        return static_cast<wf_amplitude_t>(rand() % WF_AMPLITUDE_MAX);
    }

    return original_amplitude;
}
} // namespace HyperstreamRemixer::Debug
