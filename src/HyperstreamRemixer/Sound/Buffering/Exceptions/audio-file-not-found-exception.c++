#include "audio-file-not-found-exception.h++"

#include <fmt/format.h>

namespace HyperstreamRemixer::Sound::Buffering::Exceptions {
AudioFileNotFoundException::AudioFileNotFoundException(const std::string &file_path)
    : HyperstreamException(fmt::format("Audio file {} not found. Please, check the path.", file_path)) {}
} // namespace HyperstreamRemixer::Sound::Buffering::Exceptions
