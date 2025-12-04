#pragma once
#include "HyperstreamRemixer/Audio/Waveform/waveform-types.h++"

namespace HyperstreamRemixer::Audio::Debug {
    using namespace Waveform;

    inline wf_amplitude_t add_white_noise_if(const wf_amplitude_t original_amplitude, const bool should_add_signal) {
        if (should_add_signal) {
            return rand() % WF_AMPLITUDE_MAX;
        }

        return original_amplitude;
    }
}
