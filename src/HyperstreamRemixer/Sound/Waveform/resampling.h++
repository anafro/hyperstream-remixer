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

inline auto interpolate_cubic_hermite(const wf_amplitude_t a, const wf_amplitude_t b, const wf_amplitude_t c, const wf_amplitude_t d, const double f) -> wf_amplitude_t {
    const auto m0 = static_cast<wf_amplitude_t>(static_cast<double>(c - a) * 0.5);
    const auto m1 = static_cast<wf_amplitude_t>(static_cast<double>(d - b) * 0.5);
    const auto f2 = f * f;
    const auto f3 = f * f * f;

    return ((2 * f3 - 3 * f2 + 1) * b) +
           ((f3 - 2 * f2 + f) * m0) +
           ((-2 * f3 + 3 * f2) * c) +
           ((f3 - f2) * m1);
}
} // namespace HyperstreamRemixer::Sound::Waveform
