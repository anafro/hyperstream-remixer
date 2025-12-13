#pragma once
#include "HyperstreamRemixer/Sound/Waveform/waveform-types.h++"
#include "HyperstreamRemixer/Memory/allocation.h++"


using HyperstreamRemixer::Sound::Waveform::wf_amplitude_t;
using HyperstreamRemixer::Sound::Waveform::wf_samples_t;
using HyperstreamRemixer::Sound::Waveform::wf_channels_t;
using HyperstreamRemixer::Sound::Waveform::wf_sample_rate_t;

namespace HyperstreamRemixer::Sound::Effects {
    using namespace Memory;

    class AudioEffect {
    public:
        virtual ~AudioEffect() = default;
        virtual void apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) = 0;
    };
}
