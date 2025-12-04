#pragma once
#include "HyperstreamRemixer/Exceptions/hyperstream-exception.h++"

namespace HyperstreamRemixer::Audio::Exceptions {
    class SDLInitException final : public HyperstreamRemixer::Exceptions::HyperstreamException {
    public:
        explicit SDLInitException(const std::string& message);
    };
}
