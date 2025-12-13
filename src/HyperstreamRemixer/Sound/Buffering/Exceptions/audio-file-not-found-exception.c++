#include "audio-file-not-found-exception.h++"

#include <format>

namespace HyperstreamRemixer::Sound::Buffering::Exceptions {
    AudioFileNotFoundException::AudioFileNotFoundException(const std::string& file_path)
        : HyperstreamException(std::format("Audio file {} not found. Please, check the path.", file_path)) {}
}
