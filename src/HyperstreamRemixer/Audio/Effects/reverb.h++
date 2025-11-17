#pragma once
#include "audio-effect.h++"
#include <Freeverb/Components/revmodel.hpp>

namespace HyperstreamRemixer::Audio::Effects {
    class Reverb final : public AudioEffect {
    public:
        explicit Reverb(double decay = 1.000f);
        void apply(wf_amplitude_t *audio_buffer, wf_samples_t audio_length, wf_channels_t channels, wf_sample_rate_t sample_rate) override;

    private:
        double decay;
        revmodel freeverb;
    };
}
