#pragma once
#include "audio-effect.h++"
#include <HyperstreamRemixer/Math/clipping.h++>
#include <HyperstreamRemixer/Sugar/percents.h++>

namespace HyperstreamRemixer::Sound::Effects {
using namespace Math;
using namespace Sugar;
typedef double fx_speed_sample_t;
typedef double fx_speed_fraction_t;
typedef double fx_speed_t;
inline constinit fx_speed_t fx_speed_min = 50_percent;
inline constinit fx_speed_t fx_speed_default = 100_percent;
inline constinit fx_speed_t fx_speed_max = 400_percent;

class Speed final : public AudioEffect {
  public:
    explicit Speed(fx_speed_t speed = fx_speed_default);
    void apply(Unit<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;
    fx_speed_t speed;

  private:
    static auto clip_speed(fx_speed_t overflown_speed) -> fx_speed_t;
    static auto calculate_resampled_audio_length(wf_samples_t audio_length, fx_speed_t speed) -> wf_samples_t;
    static auto calculate_resampled_sample_index(wf_samples_t index, fx_speed_t speed) -> fx_speed_sample_t;
    static auto calculate_resampled_fraction(fx_speed_sample_t speed_sample_index, wf_samples_t index) -> fx_speed_fraction_t;
};
} // namespace HyperstreamRemixer::Sound::Effects
