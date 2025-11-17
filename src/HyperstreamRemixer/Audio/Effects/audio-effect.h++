#pragma once
#include "HyperstreamRemixer/Audio/Waveform/waveform-types.h++"


using HyperstreamRemixer::Audio::Waveform::wf_amplitude_t;
using HyperstreamRemixer::Audio::Waveform::wf_samples_t;
using HyperstreamRemixer::Audio::Waveform::wf_channels_t;
using HyperstreamRemixer::Audio::Waveform::wf_sample_rate_t;

namespace HyperstreamRemixer::Audio::Effects {
    class AudioEffect {
    public:
        virtual ~AudioEffect() = default;
        virtual void apply(wf_amplitude_t* audio_buffer, wf_samples_t audio_length, wf_channels_t channels, wf_sample_rate_t sample_rate) = 0;
    };
}
