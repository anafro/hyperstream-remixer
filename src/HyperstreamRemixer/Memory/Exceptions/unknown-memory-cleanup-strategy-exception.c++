#include "unknown-memory-cleanup-strategy-exception.h++"


namespace HyperstreamRemixer::Memory::Exceptions {
    inline UnknownMemoryCleanupStrategyException::UnknownMemoryCleanupStrategyException()
        : HyperstreamException("The cleanup strategy is unknown. See memory.h++ for allowed strategies.") {}
}
