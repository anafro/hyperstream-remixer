#include "eq-ascii-format-exception.h++"


namespace HyperstreamRemixer::Sound::Effects::Exceptions {
    EQAsciiFormatException::EQAsciiFormatException(const std::string &message)
        : HyperstreamException(message) {}
}

