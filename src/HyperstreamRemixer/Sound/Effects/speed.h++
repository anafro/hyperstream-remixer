#pragma once
#include "audio-effect.h++"
#include "HyperstreamRemixer/Debug/visual-audio-debugger.h++"
#include "HyperstreamRemixer/Math/clipping.h++"

namespace HyperstreamRemixer::Audio::Effects {
    using namespace Math;
    typedef double fx_speed_sample_t;
    typedef double fx_speed_fraction_t;
    typedef double fx_speed_t;

    class Speed final : public AudioEffect {
    public:
        explicit Speed(fx_speed_t speed);
        void apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;
    private:
        fx_speed_t speed;
        static fx_speed_t clip_speed(fx_speed_t overflown_speed);
        static wf_samples_t calculate_resampled_audio_length(wf_samples_t audio_length, fx_speed_t speed);
        static fx_speed_sample_t calculate_resampled_sample_index(wf_samples_t index, fx_speed_t speed);
        static fx_speed_fraction_t calculate_resampled_fraction(fx_speed_sample_t speed_sample_index, wf_samples_t index);
        REMIXER_VISUAL_DEBUGGER_FRIEND_DEFINITION();
    };
}
