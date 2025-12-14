#pragma once
#include "audio-effect.h++"
#include <Freeverb/Components/revmodel.hpp>
#include <HyperstreamRemixer/Sugar/percents.h++>

namespace HyperstreamRemixer::Sound::Effects {
using namespace Sugar;

using fx_reverb_t = double;
using fx_reverb_dry_t = float;
using fx_reverb_roomsize_t = float;
using fx_reverb_width_t = float;
using fx_reverb_damp_t = float;
using fx_reverb_mode_t = float;

inline constexpr fx_reverb_t fx_reverb_min = 0_percent;
inline constexpr fx_reverb_t fx_reverb_default = 0_percent;
inline constexpr fx_reverb_t fx_reverb_max = 100_percent;

inline constexpr fx_reverb_t fx_reverb_dry_min = 0_percent;
inline constexpr fx_reverb_t fx_reverb_dry_default = 40_percent;
inline constexpr fx_reverb_t fx_reverb_dry_max = 100_percent;

inline constexpr fx_reverb_t fx_reverb_roomsize_min = 0_percent;
inline constexpr fx_reverb_t fx_reverb_roomsize_default = 66_percent;
inline constexpr fx_reverb_t fx_reverb_roomsize_max = 100_percent;

inline constexpr fx_reverb_t fx_reverb_width_min = 0_percent;
inline constexpr fx_reverb_t fx_reverb_width_default = 20_percent;
inline constexpr fx_reverb_t fx_reverb_width_max = 100_percent;

inline constexpr fx_reverb_t fx_reverb_damp_min = 0_percent;
inline constexpr fx_reverb_t fx_reverb_damp_default = 40_percent;
inline constexpr fx_reverb_t fx_reverb_damp_max = 100_percent;

inline constexpr fx_reverb_t fx_reverb_mode_min = 0_percent;
inline constexpr fx_reverb_t fx_reverb_mode_default = 0_percent;
inline constexpr fx_reverb_t fx_reverb_mode_max = 100_percent;

class Reverb final : public AudioEffect {
  public:
    explicit Reverb(fx_reverb_t reverb = fx_reverb_default);
    void apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;
    fx_reverb_t reverb;

  private:
    revmodel freeverb;
};
} // namespace HyperstreamRemixer::Sound::Effects
