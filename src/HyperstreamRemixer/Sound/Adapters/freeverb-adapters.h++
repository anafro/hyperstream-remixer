#pragma once
#include <iostream>
#include <limits>

#include "HyperstreamRemixer/Sound/Waveform/amplitude-clipping.h++"
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"
#include "HyperstreamRemixer/Math/clipping.h++"
#include "HyperstreamRemixer/Math/sgn.h++"


namespace HyperstreamRemixer::Sound::Adapters {
    using namespace Waveform;
    using namespace Math;
    typedef float fv_amplitude_t;
    typedef long fv_samples_t;
    constexpr fv_amplitude_t FV_AMPLITUDE_MAX = +1.000f;
    constexpr fv_amplitude_t FV_AMPLITUDE_MIN = -1.000f;
    constexpr fv_samples_t FV_SAMPLES_MAX = std::numeric_limits<fv_samples_t>::max();

    [[nodiscard]]
    inline fv_amplitude_t fv_clip_amplitude(const fv_amplitude_t fv_overflown_amplitude) {
        return clip(FV_AMPLITUDE_MIN, fv_overflown_amplitude, FV_AMPLITUDE_MAX);
    }

    [[nodiscard]]
    inline fv_amplitude_t fv_amplitude(const wf_amplitude_t wf_amplitude) {
        return fv_clip_amplitude(static_cast<fv_amplitude_t>(wf_amplitude) / (static_cast<fv_amplitude_t>(std::numeric_limits<wf_amplitude_t>::max()) + 1));
    }

    [[nodiscard]]
    inline wf_amplitude_t from_fv_amplitude(fv_amplitude_t fv_amplitude) {
        fv_amplitude = fv_clip_amplitude(fv_amplitude);
        const wf_amplitude_t wf_amplitude = clip_amplitude(static_cast<wf_amplitude_t>(fv_amplitude * static_cast<fv_amplitude_t>(std::numeric_limits<wf_amplitude_t>::max() - 1)));

        return wf_amplitude;
    }

    [[nodiscard]]
    constexpr fv_samples_t fv_samples(const wf_samples_t wf_samples) {
        if (wf_samples > FV_SAMPLES_MAX) {
            return FV_SAMPLES_MAX;
        }

        return static_cast<fv_samples_t>(wf_samples);
    }
}
