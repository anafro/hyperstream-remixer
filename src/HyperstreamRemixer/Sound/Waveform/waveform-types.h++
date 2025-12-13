#pragma once
#include <cstdint>
#include <limits>

namespace HyperstreamRemixer::Sound::Waveform {
    typedef int16_t wf_amplitude_t;
    typedef int32_t wf_overflown_amplitude_t;
    typedef std::size_t wf_samples_t;
    typedef std::size_t wf_channels_t;
    typedef int32_t wf_sample_rate_t;
    typedef int32_t wf_frequency_t;
    typedef int32_t wf_size_t;

    constexpr wf_amplitude_t WF_AMPLITUDE_MAX = std::numeric_limits<wf_amplitude_t>::max();
}
