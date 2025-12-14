#pragma once
#include "HyperstreamRemixer/Arrays/safe-access.h++"
#include "HyperstreamRemixer/Math/factor-clipping.h++"
#include "HyperstreamRemixer/Sound/Waveform/sample-rate.h++"
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"

namespace HyperstreamRemixer::Sound::Filters {
using namespace Arrays;
using namespace Waveform;
using namespace Math;

[[nodiscard]]
inline auto sample_comb(const wf_amplitude_t *const audio_buffer,
                        const wf_sample_rate_t sample_rate,
                        const wf_samples_t sample_index,
                        const std::chrono::milliseconds delay,
                        const double feedback) -> wf_amplitude_t {
    const wf_samples_t delay_samples = duration_to_samples(delay, sample_rate);

    if (sample_index < delay_samples) {
        return audio_buffer[sample_index];
    }

    return static_cast<wf_amplitude_t>(audio_buffer[sample_index] + feedback * audio_buffer[sample_index - delay_samples]);
}
} // namespace HyperstreamRemixer::Sound::Filters
