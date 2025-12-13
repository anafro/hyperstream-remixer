#pragma once
#include "HyperstreamRemixer/Exceptions/hyperstream-exception.h++"

namespace HyperstreamRemixer::Memory::Exceptions {
    class UnknownMemoryCleanupStrategyException final : public HyperstreamRemixer::Exceptions::HyperstreamException {
    public:
        explicit UnknownMemoryCleanupStrategyException();
    };
}
