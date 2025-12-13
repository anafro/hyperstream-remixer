#include "speed.h++"

#include <HyperstreamRemixer/Sound/Waveform/resampling.h++>
#include <cmath>

namespace HyperstreamRemixer::Sound::Effects {
using namespace Waveform;

Speed::Speed(const fx_speed_t speed)
    : speed(1 / clip_speed(speed)) {}

void Speed::apply(Allocation<wf_amplitude_t> &audio_buffer, const wf_channels_t channels, wf_sample_rate_t sample_rate) {
    const auto old_audio_length = audio_buffer.get_length<wf_samples_t>();
    const auto new_audio_length = calculate_resampled_audio_length(old_audio_length, speed);
    const auto *old_audio_buffer = audio_buffer.raw();
    auto *new_audio_buffer = new wf_amplitude_t[new_audio_length];

    for (wf_samples_t old_i = 0; old_i < old_audio_length; old_i++) {
        const auto mapped_i = calculate_resampled_sample_index(old_i, speed);
        const wf_samples_t new_i = std::ceil(static_cast<wf_samples_t>(mapped_i));
        const wf_amplitude_t a = (old_i >= 1) ? old_audio_buffer[old_i - 1] : 0;
        const wf_amplitude_t b = old_audio_buffer[old_i];
        const wf_amplitude_t c = (old_i + 1 < old_audio_length) ? old_audio_buffer[old_i + 1] : 0;
        const wf_amplitude_t d = (old_i + 2 < old_audio_length) ? old_audio_buffer[old_i + 2] : 0;
        const auto f = calculate_resampled_fraction(mapped_i, new_i);
        const auto resampled_amplitude = interpolate_catmull_rom(a, b, c, d, f);

        new_audio_buffer[new_i] = resampled_amplitude;
    }

    audio_buffer.replace(new_audio_buffer, new_audio_length, CLEANUP_WITH_DELETE_1D_ARRAY);
}

fx_speed_t Speed::clip_speed(const fx_speed_t overflown_speed) {
    return clip(fx_speed_min, overflown_speed, 16.000);
}

wf_samples_t Speed::calculate_resampled_audio_length(const wf_samples_t audio_length, const fx_speed_t speed) {
    return std::ceil<wf_samples_t>(audio_length * speed);
}

fx_speed_t Speed::calculate_resampled_sample_index(const wf_samples_t audio_length, const fx_speed_t speed) {
    return audio_length * speed;
}

fx_speed_fraction_t Speed::calculate_resampled_fraction(fx_speed_sample_t speed_sample_index, wf_samples_t index) {
    return speed_sample_index - index;
}
} // namespace HyperstreamRemixer::Sound::Effects
