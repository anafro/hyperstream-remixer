#include "unknown-memory-cleanup-strategy-exception.h++"

namespace HyperstreamRemixer::Memory::Exceptions {
UnknownMemoryCleanupStrategyException::UnknownMemoryCleanupStrategyException()
    : HyperstreamException("The cleanup strategy is unknown. See memory.h++ for allowed strategies.") {}
} // namespace HyperstreamRemixer::Memory::Exceptions
