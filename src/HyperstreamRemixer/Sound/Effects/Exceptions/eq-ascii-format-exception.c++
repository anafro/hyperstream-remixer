#include "eq-ascii-format-exception.h++"


namespace HyperstreamRemixer::Audio::Effects::Exceptions {
    EQAsciiFormatException::EQAsciiFormatException(const std::string &message)
        : HyperstreamException(message) {}
}

