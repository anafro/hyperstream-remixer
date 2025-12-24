#include "reverb.h++"

#include "HyperstreamRemixer/Sound/Adapters/freeverb-adapters.h++"
#include <vector>

namespace HyperstreamRemixer::Sound::Effects {
using namespace std::string_literals;
using namespace Waveform;
using namespace Adapters;

Reverb::Reverb(const fx_reverb_t reverb) : reverb(reverb) {
    this->update_freeverb_params();
}

void Reverb::apply(Allocation<wf_amplitude_t> &audio_buffer, const wf_channels_t channels, const wf_sample_rate_t sample_rate) {
    this->update_freeverb_params();
    const auto audio_length = audio_buffer.get_length<wf_samples_t>();
    std::vector<fv_amplitude_t> fv_in_left(audio_length);
    std::vector<fv_amplitude_t> fv_in_right(audio_length);
    std::vector<fv_amplitude_t> fv_out_left(audio_length);
    std::vector<fv_amplitude_t> fv_out_right(audio_length);
    const fv_samples_t fv_audio_length = fv_samples(audio_length);

    for (wf_samples_t sample_index = 0; sample_index < audio_length; sample_index++) {
        const fv_amplitude_t amplitude = fv_amplitude((*audio_buffer)[sample_index]);
        fv_in_left[sample_index] = amplitude;
        fv_in_right[sample_index] = amplitude;
    }

    freeverb.processmix(fv_in_left.data(), fv_in_right.data(), fv_out_left.data(), fv_out_right.data(), fv_audio_length, 1);

    for (wf_samples_t sample_index = 0; sample_index < audio_length; sample_index++) {
        audio_buffer.raw()[sample_index] = from_fv_amplitude(fv_out_left[sample_index]);
    }
}

void Reverb::update_freeverb_params() {
    freeverb.setwet(static_cast<float>(reverb));
    freeverb.setdry(1 - freeverb.getwet());
    freeverb.setroomsize(fx_reverb_roomsize_default);
    freeverb.setwidth(fx_reverb_width_default);
    freeverb.setdamp(fx_reverb_damp_default);
    freeverb.setmode(fx_reverb_mode_default);
}
} // namespace HyperstreamRemixer::Sound::Effects
