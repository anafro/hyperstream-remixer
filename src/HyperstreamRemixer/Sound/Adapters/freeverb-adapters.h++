#pragma once
#include <iostream>
#include <limits>

#include "HyperstreamRemixer/Math/clipping.h++"
#include "HyperstreamRemixer/Math/sgn.h++"
#include "HyperstreamRemixer/Sound/Waveform/amplitude-clipping.h++"
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"

namespace HyperstreamRemixer::Sound::Adapters {
using namespace Waveform;
using namespace Math;
typedef float fv_amplitude_t;
typedef long fv_samples_t;
constexpr fv_amplitude_t FV_AMPLITUDE_MAX = +1.000f;
constexpr fv_amplitude_t FV_AMPLITUDE_MIN = -1.000f;
constexpr fv_samples_t FV_SAMPLES_MAX = std::numeric_limits<fv_samples_t>::max();

[[nodiscard]]
inline auto fv_clip_amplitude(const fv_amplitude_t fv_overflown_amplitude) -> fv_amplitude_t {
    return clip(FV_AMPLITUDE_MIN, fv_overflown_amplitude, FV_AMPLITUDE_MAX);
}

[[nodiscard]]
inline auto fv_amplitude(const wf_amplitude_t wf_amplitude) -> fv_amplitude_t {
    return fv_clip_amplitude(static_cast<fv_amplitude_t>(wf_amplitude) / (static_cast<fv_amplitude_t>(std::numeric_limits<wf_amplitude_t>::max()) + 1));
}

[[nodiscard]]
inline auto from_fv_amplitude(fv_amplitude_t fv_amplitude) -> wf_amplitude_t {
    fv_amplitude = fv_clip_amplitude(fv_amplitude);
    const wf_amplitude_t wf_amplitude = clip_amplitude(static_cast<wf_amplitude_t>(fv_amplitude * static_cast<fv_amplitude_t>(std::numeric_limits<wf_amplitude_t>::max() - 1)));

    return wf_amplitude;
}

[[nodiscard]]
constexpr auto fv_samples(const wf_samples_t wf_samples) -> fv_samples_t {
    if (wf_samples > FV_SAMPLES_MAX) {
        return FV_SAMPLES_MAX;
    }

    return static_cast<fv_samples_t>(wf_samples);
}
} // namespace HyperstreamRemixer::Sound::Adapters
