#pragma once
#include "audio-effect.h++"
#include <Freeverb/Components/revmodel.hpp>

#include "HyperstreamRemixer/Debug/visual-audio-debugger.h++"

namespace HyperstreamRemixer::Audio::Effects {
    typedef double fx_reverb_wet_t;

    class Reverb final : public AudioEffect {
    public:
        explicit Reverb();
        void apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;

    private:
        revmodel freeverb;
        REMIXER_VISUAL_DEBUGGER_FRIEND_DEFINITION();
    };
}
