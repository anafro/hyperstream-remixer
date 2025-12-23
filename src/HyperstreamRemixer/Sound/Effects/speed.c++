#include "speed.h++"
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"

#include <HyperstreamRemixer/Arrays/safe-access.h++>
#include <HyperstreamRemixer/Sound/Waveform/resampling.h++>
#include <cmath>

namespace HyperstreamRemixer::Sound::Effects {
using namespace Waveform;
using namespace Arrays;
Speed::Speed(const fx_speed_t speed) : speed(clip_speed(1 / speed)) {}

void Speed::apply(Allocation<wf_amplitude_t> &audio_buffer, const wf_channels_t channels, wf_sample_rate_t sample_rate) {
    const auto old_audio_length = audio_buffer.get_length<wf_samples_t>();
    const auto new_audio_length = calculate_resampled_audio_length(old_audio_length, speed);
    const auto *old_audio_buffer = audio_buffer.raw();
    auto *new_audio_buffer = new wf_amplitude_t[new_audio_length];

    for (wf_samples_t resampled_i = 0; resampled_i < new_audio_length; resampled_i++) {
        const fx_speed_fraction_t original_floating_i = static_cast<fx_speed_fraction_t>(resampled_i) / speed;
        const wf_samples_t original_i = std::floor(original_floating_i);
        const wf_amplitude_t amplitude_m1 = safe_get(old_audio_buffer, original_i - 1, old_audio_length, old_audio_buffer[0]);
        const wf_amplitude_t amplitude_0 = safe_get(old_audio_buffer, original_i, old_audio_length, old_audio_buffer[0]);
        const wf_amplitude_t amplitude_1 = safe_get(old_audio_buffer, original_i + 1, old_audio_length, amplitude_0);
        const wf_amplitude_t amplitude_2 = safe_get(old_audio_buffer, original_i + 2, old_audio_length, amplitude_1);
        const fx_speed_fraction_t resampling_factor = original_floating_i - static_cast<fx_speed_fraction_t>(original_i);
        const wf_amplitude_t resampled_amplitude = interpolate_cubic_hermite(amplitude_m1, amplitude_0, amplitude_1, amplitude_2, resampling_factor);

        safe_set(new_audio_buffer, resampled_i, new_audio_length, resampled_amplitude);
    }

    audio_buffer.replace(new_audio_buffer, new_audio_length * sizeof(wf_amplitude_t), CLEANUP_WITH_DELETE_1D_ARRAY);
}

auto Speed::clip_speed(const fx_speed_t overflown_speed) -> fx_speed_t {
    return clip(fx_speed_min, overflown_speed, 16.000);
}

auto Speed::calculate_resampled_audio_length(const wf_samples_t audio_length, const fx_speed_t speed) -> wf_samples_t {
    return std::ceil<wf_samples_t>(audio_length * speed);
}

auto Speed::calculate_resampled_sample_index(const wf_samples_t audio_length, const fx_speed_t speed) -> fx_speed_t {
    return audio_length * speed;
}

auto Speed::calculate_resampled_fraction(fx_speed_sample_t speed_sample_index, wf_samples_t index) -> fx_speed_fraction_t {
    return speed_sample_index - index;
}
} // namespace HyperstreamRemixer::Sound::Effects
