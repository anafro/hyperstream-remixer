#pragma once
#include <HyperstreamRemixer/Exceptions/hyperstream-exception.h++>

namespace HyperstreamRemixer::Sound::Buffering::Exceptions {
    using namespace HyperstreamRemixer::Exceptions;

    class AudioFileNotFoundException final : public HyperstreamException {
    public:
        explicit AudioFileNotFoundException(const std::string &file_path);
    };
}