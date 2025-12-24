#pragma once
#include "HyperstreamRemixer/Sound/Effects/audio-effect.h++"
#include "HyperstreamRemixer/Sugar/percents.h++"

namespace HyperstreamRemixer::Sound::Effects {
using namespace Sugar;
using fx_lowpass_parameter_t = double;
const constexpr fx_lowpass_parameter_t fx_lowpass_cutoff_min = 0_percent;
const constexpr fx_lowpass_parameter_t fx_lowpass_cutoff_default = 40_percent;
const constexpr fx_lowpass_parameter_t fx_lowpass_cutoff_max = 200_percent;
const constexpr fx_lowpass_parameter_t butterworth_quality = 0.70710678118654752440F;

class Lowpass final : public AudioEffect {
  public:
    Lowpass(fx_lowpass_parameter_t cutoff, float quality = butterworth_quality);
    fx_lowpass_parameter_t cutoff;

    Lowpass(const Lowpass &) = default;
    Lowpass(Lowpass &&) = default;
    auto operator=(const Lowpass &) -> Lowpass & = default;
    auto operator=(Lowpass &&) -> Lowpass & = default;
    void apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;

  private:
    fx_lowpass_parameter_t quality;
    fx_lowpass_parameter_t b0;
    fx_lowpass_parameter_t b1;
    fx_lowpass_parameter_t b2;
    fx_lowpass_parameter_t a1;
    fx_lowpass_parameter_t a2;
    fx_lowpass_parameter_t z1;
    fx_lowpass_parameter_t z2;
};
} // namespace HyperstreamRemixer::Sound::Effects
