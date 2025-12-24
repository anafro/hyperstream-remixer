#pragma once
#include "HyperstreamRemixer/Sound/Effects/audio-effect.h++"
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"
#include "HyperstreamRemixer/Sugar/percents.h++"

namespace HyperstreamRemixer::Sound::Effects {
using namespace Waveform;
using namespace Sugar;

using fx_crackle_probability_t = double;
using fx_crackle_steps_t = uint16_t;

constexpr wf_amplitude_t fx_crackle_shift_max = 16384;

constexpr fx_crackle_probability_t fx_crackle_probability_min = 0_percent;
constexpr fx_crackle_probability_t fx_crackle_probability_default = 20_percent;
constexpr fx_crackle_probability_t fx_crackle_probability_max = 100_percent;

constexpr fx_crackle_steps_t fx_crackle_steps_min = 2;
constexpr fx_crackle_steps_t fx_crackle_steps_default = 6;
constexpr fx_crackle_steps_t fx_crackle_steps_max = 64;

class Crackle final : public AudioEffect {
  public:
    fx_crackle_probability_t probability;
    fx_crackle_steps_t steps;
    Crackle(fx_crackle_probability_t probability = fx_crackle_probability_default, fx_crackle_steps_t steps = fx_crackle_steps_default);
    void apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;
};
} // namespace HyperstreamRemixer::Sound::Effects
