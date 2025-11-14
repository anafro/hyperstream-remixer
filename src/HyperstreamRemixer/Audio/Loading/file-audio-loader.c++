#include "file-audio-loader.h++"

namespace HyperstreamRemixer::Audio::Loading {
    FileAudioLoader::FileAudioLoader() {

    }

    FileAudioLoader::~FileAudioLoader() {
        SDL_ClearAudioStream(this->audio_stream);
    }

    void FileAudioLoader::load_audio_stream() {

    }
}
