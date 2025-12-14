#pragma once
#include "audio-effect.h++"
#include <Freeverb/Components/revmodel.hpp>
#include <HyperstreamRemixer/Sugar/percents.h++>

namespace HyperstreamRemixer::Sound::Effects {
using namespace Sugar;

typedef double fx_reverb_t;
inline constinit fx_reverb_t fx_reverb_min = 0_percent;
inline constinit fx_reverb_t fx_reverb_default = 0_percent;
inline constinit fx_reverb_t fx_reverb_max = 100_percent;

class Reverb final : public AudioEffect {
  public:
    explicit Reverb(fx_reverb_t reverb = fx_reverb_default);
    void apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;
    fx_reverb_t reverb;

  private:
    revmodel freeverb;
};
} // namespace HyperstreamRemixer::Sound::Effects
