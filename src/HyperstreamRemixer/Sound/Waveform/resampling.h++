#pragma once
#include "HyperstreamRemixer/Sound/Waveform/amplitude-clipping.h++"
#include "waveform-types.h++"
#include <cmath>

namespace HyperstreamRemixer::Sound::Waveform {
inline auto interpolate_catmull_rom(
    const wf_amplitude_t a,
    const wf_amplitude_t b,
    const wf_amplitude_t c,
    const wf_amplitude_t d,
    const double f) -> wf_amplitude_t {
    const auto oa = static_cast<wf_overflown_amplitude_t>(a);
    const auto ob = static_cast<wf_overflown_amplitude_t>(b);
    const auto oc = static_cast<wf_overflown_amplitude_t>(c);
    const auto od = static_cast<wf_overflown_amplitude_t>(d);
    return clip_amplitude(.5 * ((2 * ob) + (-oa + oc) * f + (2 * oa - 5 * ob + 4 * oc - od) * f * f + (-oa + 3 * ob - 3 * oc + od) * f * f * f));
}

inline auto interpolate_lerp(
    const wf_amplitude_t a,
    const wf_amplitude_t b,
    const double f) -> wf_amplitude_t {
    return clip_amplitude(static_cast<wf_overflown_amplitude_t>(a) + static_cast<wf_overflown_amplitude_t>(f * (static_cast<wf_overflown_amplitude_t>(b) - a)));
}
} // namespace HyperstreamRemixer::Sound::Waveform
