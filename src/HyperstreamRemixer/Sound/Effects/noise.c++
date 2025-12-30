#include "noise.h++"
#include "HyperstreamRemixer/Sound/Waveform/amplitude-clipping.h++"
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"
#include <cmath>

namespace HyperstreamRemixer::Sound::Effects {
using namespace Waveform;
Noise::Noise(fx_noise_strength_t strength)
    : strength(strength) {}

void Noise::apply(Unit<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) {
    const auto audio_length = audio_buffer.length();
    for (wf_samples_t i = 0; i < audio_length; i++) {
        const auto noise_shift = static_cast<wf_overflown_amplitude_t>(rand() % fx_noise_shift_max * this->strength * sin(std::numbers::pi * static_cast<double>(i) / static_cast<double>(audio_length)));
        (*audio_buffer)[i] = clip_amplitude(static_cast<wf_overflown_amplitude_t>((*audio_buffer)[i]) + noise_shift);
    }
}
} // namespace HyperstreamRemixer::Sound::Effects
