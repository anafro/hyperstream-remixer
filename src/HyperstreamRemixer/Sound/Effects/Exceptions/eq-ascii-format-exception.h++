#pragma once
#include <HyperstreamRemixer/Exceptions/hyperstream-exception.h++>

namespace HyperstreamRemixer::Audio::Effects::Exceptions {
    using namespace HyperstreamRemixer::Exceptions;

    class EQAsciiFormatException final : public HyperstreamException {
    public:
        explicit EQAsciiFormatException(const std::string &message);
    };
}