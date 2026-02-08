#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>

namespace HyperstreamRemixer::Sound::Waveform {
using wf_amplitude_t = int16_t;
using wf_overflown_amplitude_t = int32_t;
using wf_samples_t = std::size_t;
using wf_channels_t = std::size_t;
using wf_sample_rate_t = int32_t;
using wf_frequency_t = int32_t;
using wf_size_t = int32_t;

constexpr wf_amplitude_t WF_AMPLITUDE_MAX = std::numeric_limits<wf_amplitude_t>::max();
} // namespace HyperstreamRemixer::Sound::Waveform
