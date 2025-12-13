#pragma once
#include <chrono>

#include "waveform-types.h++"


namespace HyperstreamRemixer::Sound::Waveform {
    inline wf_samples_t duration_to_samples(const std::chrono::milliseconds duration, const wf_sample_rate_t sample_rate) {
        return static_cast<wf_samples_t>(duration.count() * sample_rate / 1000);
    }
}
