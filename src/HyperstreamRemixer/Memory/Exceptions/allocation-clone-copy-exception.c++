#include "allocation-clone-copy-exception.h++"

namespace HyperstreamRemixer::Memory::Exceptions {
AllocationCloneCopyException::AllocationCloneCopyException(const std::string &message)
    : HyperstreamException(message) {}
} // namespace HyperstreamRemixer::Memory::Exceptions
