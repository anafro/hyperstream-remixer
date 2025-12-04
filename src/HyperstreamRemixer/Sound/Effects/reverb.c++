#include "reverb.h++"

#include "HyperstreamRemixer/Audio/Filters/comb.h++"
#include "HyperstreamRemixer/Audio/Adapters/freeverb-adapters.h++"
#include <HyperstreamRemixer/Debug/waveform-signals.h++>
#include <HyperstreamRemixer/Sugar/conditions.h++>
#include <vector>

namespace HyperstreamRemixer::Audio::Effects {
    using namespace std::chrono_literals;
    using namespace Waveform;
    using namespace Adapters;
    using namespace Debug;
    using namespace Sugar;

    Reverb::Reverb() {
        freeverb.setwet(0.200f);
        freeverb.setdry(0.400f);
        freeverb.setroomsize(0.660f);
        freeverb.setwidth(0.200f);
        freeverb.setdamp(0.400f);
        freeverb.setmode(0.000f);
    }

    void Reverb::apply(Allocation<wf_amplitude_t> &audio_buffer, const wf_channels_t channels, const wf_sample_rate_t sample_rate) {
        const auto audio_length = audio_buffer.get_length<wf_samples_t>() * channels;
        std::vector<fv_amplitude_t> fv_in_left(audio_length);
        std::vector<fv_amplitude_t> fv_in_right(audio_length);
        std::vector<fv_amplitude_t> fv_out_left(audio_length);
        std::vector<fv_amplitude_t> fv_out_right(audio_length);
        const fv_samples_t fv_audio_length = fv_samples(audio_length);

        for (wf_samples_t sample_index = 0; sample_index < audio_length; sample_index++) {
            const fv_amplitude_t amplitude = fv_amplitude(audio_buffer.raw()[sample_index]);
            fv_in_left[sample_index] = amplitude;
            fv_in_right[sample_index] = amplitude;
        }

        freeverb.processmix(fv_in_left.data(), fv_in_right.data(), fv_out_left.data(), fv_out_right.data(), fv_audio_length, 1);

        for (wf_samples_t sample_index = 0; sample_index < audio_length; sample_index++) {
            audio_buffer.raw()[sample_index] = from_fv_amplitude(fv_out_left[sample_index]);
        }
    }
}
