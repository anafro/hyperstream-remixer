#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "HyperstreamRemixer/Audio/Waveform/hz-clipping.h++"
#include "HyperstreamRemixer/Audio/Waveform/waveform-types.h++"


namespace HyperstreamRemixer::Sound::Filters {
    using namespace Waveform;

    inline void low_pass(wf_amplitude_t* audio_buffer, const wf_samples_t sample_index, const wf_sample_rate_t sample_rate, float cutoff_hz) {
        cutoff_hz = clip_hz(cutoff_hz);
        const float dt = 1.000f / static_cast<float>(sample_rate);
        const float rc = 1.000f / (2.000f * static_cast<float>(M_PI) * cutoff_hz);
        const float alpha = dt / (rc + dt);

        if (sample_index == 0) {
            return;
        }

        audio_buffer[sample_index] = static_cast<wf_amplitude_t>(static_cast<float>(audio_buffer[sample_index - 1]) + alpha * static_cast<float>(audio_buffer[sample_index] - audio_buffer[sample_index - 1]));
    }
}
