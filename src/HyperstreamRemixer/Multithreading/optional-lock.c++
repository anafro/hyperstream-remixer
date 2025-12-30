#include "optional-lock.h++"
#include <mutex>

namespace HyperstreamRemixer::Multithreading {
OptionalLock::OptionalLock(std::recursive_timed_mutex *nullable_mutex)
    : nullable_mutex(nullable_mutex) {
    if (nullable_mutex != nullptr) {
        nullable_mutex->lock();
    }
}

OptionalLock::~OptionalLock() {
    if (nullable_mutex != nullptr) {
        nullable_mutex->lock();
    }
}
} // namespace HyperstreamRemixer::Multithreading
