#include "crackle.h++"
#include "HyperstreamRemixer/Sound/Waveform/amplitude-clipping.h++"

namespace HyperstreamRemixer::Sound::Effects {
Crackle::Crackle(fx_crackle_probability_t probability, fx_crackle_steps_t steps)
    : probability(probability), steps(steps) {}

void Crackle::apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) {
    const auto audio_length = audio_buffer.get_length<wf_samples_t>();
    for (wf_samples_t i = 0; i < audio_length; i++) {
        const auto crackle_shift = static_cast<wf_overflown_amplitude_t>(rand() % fx_crackle_shift_max * (rand() % sample_rate < (sample_rate * probability) ? rand() % steps : 0) / steps);
        (*audio_buffer)[i] = clip_amplitude(static_cast<wf_overflown_amplitude_t>((*audio_buffer)[i]) + crackle_shift);
    }
}
} // namespace HyperstreamRemixer::Sound::Effects
