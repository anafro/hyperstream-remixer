#pragma once
#include "HyperstreamRemixer/Exceptions/hyperstream-exception.h++"

namespace HyperstreamRemixer::Audio::Exceptions {
    class AlreadyInitializedException final : public HyperstreamRemixer::Exceptions::HyperstreamException {

    public:
        explicit AlreadyInitializedException();
    };
}
