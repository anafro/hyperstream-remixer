#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <HyperstreamRemixer/Audio/Effects/audio-effect.h++>
#include <HyperstreamRemixer/Macros/coerce-inline.h++>

namespace HyperstreamRemixer::Audio::Buffering {
    typedef std::byte audio_fx_behavior_t;
    constexpr audio_fx_behavior_t APPLY_FX_NOW { 0x00 };
    constexpr audio_fx_behavior_t APPLY_FX_ON_PLAY { 0x01 };

    using namespace Waveform;
    using namespace Effects;

    class Audio final {
    public:
        Audio(
            std::vector<AudioEffect*> &&effects,
            SDL_AudioFormat audio_format,
            Allocation<wf_amplitude_t>&& buffer,
            wf_frequency_t frequency,
            wf_channels_t channels,
            audio_fx_behavior_t fx_behavior = APPLY_FX_NOW
        );

        ~Audio();
        void play() const;
        static Audio* from_mp3_file(std::vector<AudioEffect*> &&effects, const std::string& file_path);

    protected:
        std::vector<AudioEffect*> effects;
        SDL_AudioFormat audio_format;
        Allocation<wf_amplitude_t> original_buffer;
        Allocation<wf_amplitude_t> buffer_with_effects;
        wf_frequency_t frequency;
        wf_channels_t channels;
        audio_fx_behavior_t fx_behavior;

    private:
        Allocation<wf_amplitude_t> apply_effects(Allocation<wf_amplitude_t>& buffer) const;
        __REMIXER_COERCE_INLINE wf_samples_t get_samples() const;
    };
}
