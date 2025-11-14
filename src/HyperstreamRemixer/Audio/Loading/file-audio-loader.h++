#pragma once
#include "audio-loader.h++"

namespace HyperstreamRemixer::Audio::Loading {
    class FileAudioLoader final : public AudioLoader {
    public:
        FileAudioLoader();
        ~FileAudioLoader() override;
        void load_audio_stream() override;
    };
}
