#pragma once
#include "HyperstreamRemixer/Exceptions/hyperstream-exception.h++"

namespace HyperstreamRemixer::Audio::Exceptions {
    class NotInitializedException final : public HyperstreamRemixer::Exceptions::HyperstreamException {
    public:
        explicit NotInitializedException();
    };
}
