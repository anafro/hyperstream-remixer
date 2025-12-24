#include "HyperstreamRemixer/Sound/Effects/audio-effect.h++"
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"
#include <HyperstreamRemixer/Sugar/percents.h++>

namespace HyperstreamRemixer::Sound::Effects {
using namespace Sugar;
using namespace Waveform;
using fx_noise_strength_t = double;
const constexpr fx_noise_strength_t fx_noise_strength_min = 0_percent;
const constexpr fx_noise_strength_t fx_noise_strength_default = 0_percent;
const constexpr fx_noise_strength_t fx_noise_strength_max = 100_percent;
const constexpr wf_amplitude_t fx_noise_shift_max = 8192;

class Noise final : public AudioEffect {
  public:
    fx_noise_strength_t strength;
    Noise(fx_noise_strength_t strength = fx_noise_strength_default);
    void apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;
};
} // namespace HyperstreamRemixer::Sound::Effects
