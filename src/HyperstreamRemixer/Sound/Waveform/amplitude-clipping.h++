#pragma once
#include <limits>


namespace HyperstreamRemixer::Sound::Waveform {
    inline wf_amplitude_t clip_amplitude(const wf_overflown_amplitude_t overflown_amplitude) {
        constexpr wf_amplitude_t max_amplitude = std::numeric_limits<wf_amplitude_t>::max();
        constexpr wf_amplitude_t min_amplitude = std::numeric_limits<wf_amplitude_t>::min();

        if (overflown_amplitude > max_amplitude) {
            return max_amplitude;
        }

        if (overflown_amplitude < min_amplitude) {
            return min_amplitude;
        }

        return static_cast<wf_amplitude_t>(overflown_amplitude);
    }
}