#pragma once
#include <HyperstreamRemixer/Exceptions/hyperstream-exception.h++>

namespace HyperstreamRemixer::Memory::Exceptions {
class AllocationCloneCopyException final : public HyperstreamRemixer::Exceptions::HyperstreamException {
  public:
    explicit AllocationCloneCopyException(const std::string &message);
};
} // namespace HyperstreamRemixer::Memory::Exceptions
