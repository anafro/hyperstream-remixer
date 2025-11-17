#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "HyperstreamRemixer/Audio/Effects/audio-effect.h++"

namespace HyperstreamRemixer::Audio::Buffering {
    using namespace Waveform;
    using namespace Effects;

    typedef uint8_t audio_buffer_cleanup_strategy_t;
    constexpr audio_buffer_cleanup_strategy_t AUDIO_BUFFER_DONT_CLEANUP = 0b00000000;
    constexpr audio_buffer_cleanup_strategy_t AUDIO_BUFFER_DELETE_ARRAY  = 0b00000001;
    constexpr audio_buffer_cleanup_strategy_t AUDIO_BUFFER_INVOKE_FREE   = 0b00000010;

    class Audio final {
    public:
        Audio(
            const std::vector<AudioEffect*> &effects,
            SDL_AudioFormat audio_format,
            wf_amplitude_t* buffer,
            wf_frequency_t frequency,
            wf_samples_t samples,
            wf_channels_t channels,
            audio_buffer_cleanup_strategy_t buffer_cleanup_strategy
        );
        ~Audio();
        void play() const;
        static Audio* from_mp3_file(const std::vector<AudioEffect*> &effects, const std::string& file_path);

    protected:
        std::vector<AudioEffect*> effects;
        SDL_AudioFormat audio_format;
        wf_amplitude_t* buffer;
        wf_frequency_t frequency;
        wf_samples_t samples;
        wf_channels_t channels;
        audio_buffer_cleanup_strategy_t buffer_cleanup_strategy;

    private:
        void apply_effects() const;
    };
}
