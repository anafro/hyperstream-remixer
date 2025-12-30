#pragma once

#include <mutex>

namespace HyperstreamRemixer::Multithreading {
class OptionalLock {
    std::recursive_timed_mutex *nullable_mutex;

  public:
    OptionalLock() = delete;
    OptionalLock(std::recursive_timed_mutex *nullable_mutex);
    ~OptionalLock();
};
} // namespace HyperstreamRemixer::Multithreading
