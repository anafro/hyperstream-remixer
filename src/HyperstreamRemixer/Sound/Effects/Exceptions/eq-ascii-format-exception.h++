#pragma once
#include <HyperstreamRemixer/Exceptions/hyperstream-exception.h++>

namespace HyperstreamRemixer::Sound::Effects::Exceptions {
    using namespace HyperstreamRemixer::Exceptions;

    class EQAsciiFormatException final : public HyperstreamException {
    public:
        explicit EQAsciiFormatException(const std::string &message);
    };
}