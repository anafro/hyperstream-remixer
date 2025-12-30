#include "lowpass.h++"
#include "HyperstreamRemixer/Sound/Waveform/amplitude-clipping.h++"
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"
#include <cmath>
#include <numbers>

namespace HyperstreamRemixer::Sound::Effects {
using namespace Waveform;

Lowpass::Lowpass(fx_lowpass_parameter_t cutoff, float quality)
    : cutoff(cutoff), quality(quality) {}

void Lowpass::apply(Unit<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) {
    this->z1 = 0.0F;
    this->z2 = 0.0F;
    const fx_lowpass_parameter_t w0 = 2 * static_cast<fx_lowpass_parameter_t>(std::numbers::pi) * cutoff;
    const fx_lowpass_parameter_t cosw = std::cos(w0);
    const fx_lowpass_parameter_t sinw = std::sin(w0);
    const fx_lowpass_parameter_t alpha = sinw / (2.0F * this->quality);
    const fx_lowpass_parameter_t a0 = 1.0F + alpha;

    this->b0 = (1.0F - cosw) * 0.5F / a0;
    this->b1 = (1.0F - cosw) / a0;
    this->b2 = b0;

    a1 = -2.0F * cosw / a0;
    a2 = (1.0F - alpha) / a0;

    const auto audio_length = audio_buffer.length();
    for (wf_samples_t i = 0; i < audio_length; i++) {
        const wf_amplitude_t original_amplitude = (*audio_buffer)[i];
        const wf_amplitude_t lowpassed_amplitude = clip_amplitude(static_cast<wf_overflown_amplitude_t>((this->b0 * static_cast<fx_lowpass_parameter_t>(original_amplitude)) + this->z1));
        this->z1 = (this->b1 * static_cast<fx_lowpass_parameter_t>(original_amplitude)) - (this->a1 * static_cast<fx_lowpass_parameter_t>(lowpassed_amplitude)) + this->z2;
        this->z1 = (this->b2 * static_cast<fx_lowpass_parameter_t>(original_amplitude)) - (this->a2 * static_cast<fx_lowpass_parameter_t>(lowpassed_amplitude));
        audio_buffer[i] = lowpassed_amplitude;
    }
}
} // namespace HyperstreamRemixer::Sound::Effects
