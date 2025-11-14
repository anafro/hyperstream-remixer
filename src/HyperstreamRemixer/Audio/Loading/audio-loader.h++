#pragma once
#include <SDL3/SDL_audio.h>

namespace HyperstreamRemixer::Audio::Loading {
    class AudioLoader {
    public:
        virtual ~AudioLoader() = 0;
        virtual void load_audio_stream() = 0;

    protected:
        SDL_AudioStream* audio_stream = nullptr;
    };
}