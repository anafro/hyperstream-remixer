#pragma once
#include <HyperstreamRemixer/Memory/Exceptions/unknown-memory-cleanup-strategy-exception.h++>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <format>
#include <type_traits>

#include <HyperstreamRemixer/Macros/coerce-inline.h++>
#include <HyperstreamRemixer/Memory/Exceptions/allocation-clone-copy-exception.h++>
#include <HyperstreamRemixer/Runtime/interrupt.h++>

namespace HyperstreamRemixer::Memory {
using namespace Exceptions;

typedef uint8_t allocation_cleanup_strategy_t;
constexpr allocation_cleanup_strategy_t DONT_CLEANUP = 0x00;
constexpr allocation_cleanup_strategy_t CLEANUP_WITH_FREE = 0x01;
constexpr allocation_cleanup_strategy_t CLEANUP_WITH_DELETE = 0x02;
constexpr allocation_cleanup_strategy_t CLEANUP_WITH_DELETE_1D_ARRAY = 0x03;

template <typename T>
class Allocation final {
  public:
    Allocation() = delete;
    Allocation(const Allocation &that);
    Allocation(Allocation &&that) noexcept;
    Allocation(T *ptr, std::size_t size,
               allocation_cleanup_strategy_t cleanup_strategy);
    ~Allocation();
    void replace(T *new_ptr, std::size_t new_size,
                 allocation_cleanup_strategy_t new_cleanup_strategy);
    [[nodiscard]] auto get_cleanup_strategy() const -> allocation_cleanup_strategy_t;
    auto operator=(const Allocation &) -> Allocation & = delete;
    auto operator=(Allocation &&that) noexcept -> Allocation &;
    auto raw() -> T *;
    auto raw() const -> const T *;
    auto operator->() -> T *;
    auto operator->() const -> const T *;
    auto operator*() -> T *;
    auto operator*() const -> const T *;
    auto create_view() -> Allocation;
    auto create_copy() const -> Allocation;
    [[nodiscard]] auto get_size() const -> std::size_t;
    template <typename S = std::size_t>
    [[nodiscard]] auto get_length() const -> S;
    [[nodiscard]] auto is_null() const -> bool;
    static auto null() -> Allocation;

  private:
    T *ptr;
    std::size_t size;
    allocation_cleanup_strategy_t cleanup_strategy;
    void cleanup();
};

template <typename T>
Allocation<T>::Allocation(const Allocation &that)
    : Allocation(that.ptr, that.size, DONT_CLEANUP) {}

template <typename T>
Allocation<T>::Allocation(T *ptr, const std::size_t size,
                          const allocation_cleanup_strategy_t cleanup_strategy)
    : ptr(ptr), size(size), cleanup_strategy(cleanup_strategy) {}

template <typename T>
Allocation<T>::~Allocation() { this->cleanup(); }

template <typename T>
Allocation<T>::Allocation(Allocation &&that) noexcept
    : ptr(that.ptr), size(that.size), cleanup_strategy(that.cleanup_strategy) {
    that.ptr = nullptr;
    that.cleanup_strategy = DONT_CLEANUP;
}

template <typename T>
void Allocation<T>::replace(
    T *new_ptr, const std::size_t new_size,
    allocation_cleanup_strategy_t new_cleanup_strategy) {
    cleanup();
    this->ptr = new_ptr;
    this->size = new_size;
    this->cleanup_strategy = new_cleanup_strategy;
}

template <typename T>
auto Allocation<T>::get_cleanup_strategy() const -> allocation_cleanup_strategy_t {
    return this->cleanup_strategy;
}

template <typename T>
auto Allocation<T>::operator=(Allocation<T> &&that) noexcept -> Allocation<T> & {
    this->cleanup();
    this->ptr = that.ptr;
    this->size = that.size;
    this->cleanup_strategy = that.cleanup_strategy;
    that.ptr = nullptr;
    that.cleanup_strategy = DONT_CLEANUP;

    return *this;
}

template <typename T>
auto Allocation<T>::raw() -> T * { return this->ptr; }

template <typename T>
auto Allocation<T>::raw() const -> const T * { return this->ptr; }

template <typename T>
auto Allocation<T>::operator->() -> T * { return raw(); }

template <typename T>
auto Allocation<T>::operator->() const -> const T * {
    return raw();
}

template <typename T>
auto Allocation<T>::operator*() -> T * { return raw(); }

template <typename T>
auto Allocation<T>::operator*() const -> const T * {
    return raw();
}

template <typename T>
auto Allocation<T>::create_view() -> Allocation<T> {
    return Allocation(this->ptr, this->size, DONT_CLEANUP);
}

template <typename T>
auto Allocation<T>::create_copy() const -> Allocation<T> {
    T *copied_ptr = nullptr;

    static_assert(std::is_copy_constructible_v<T>,
                  "Now, allocations with non-copy constructible data cannot be "
                  "clone-copied.");

    switch (cleanup_strategy) {
    case DONT_CLEANUP:
        throw AllocationCloneCopyException(
            "Can't clone_copy() allocation with no cleanup strategy. "
            "You might want to clone_copy() the original allocation "
            "or define cleanup strategy for this allocation.");
    case CLEANUP_WITH_DELETE:
        copied_ptr = new T(*this->ptr);
        break;

    case CLEANUP_WITH_DELETE_1D_ARRAY:
        copied_ptr = new T[get_length<>()];

        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memcpy(copied_ptr, this->ptr, get_size());
        } else if constexpr (std::is_copy_assignable_v<T>) {
            std::copy(this->ptr, this->ptr + get_length<>(), copied_ptr);
        } else {
            throw AllocationCloneCopyException(std::format(
                "Can't clone_copy() a 1D array allocation with T = {}, which "
                "is neither trivially copyable nor copy assignable.",
                typeid(T).name()));
        }
        break;

    case CLEANUP_WITH_FREE:
        if constexpr (!std::is_trivially_copyable_v<T>) {
            throw AllocationCloneCopyException(
                std::format("Can't clone_copy() a c-allocated allocation with "
                            "T = {}, which is not trivially copyable.",
                            typeid(T).name()));
        }

        copied_ptr = static_cast<T *>(malloc(get_size()));
        std::memcpy(copied_ptr, this->ptr, get_size());
        break;

    default:
        throw UnknownMemoryCleanupStrategyException();
    }

    return Allocation(copied_ptr, this->size, this->cleanup_strategy);
}

template <typename T>
auto Allocation<T>::get_size() const -> std::size_t {
    return this->size;
}

template <typename T>
template <typename S>
auto Allocation<T>::get_length() const -> S {
    return S(this->size / sizeof(T));
}

template <typename T>
auto Allocation<T>::is_null() const -> bool {
    return this->ptr == nullptr;
}

template <typename T>
auto Allocation<T>::null() -> Allocation<T> {
    return Allocation(nullptr, sizeof(std::nullptr_t), DONT_CLEANUP);
}

template <typename T>
void Allocation<T>::cleanup() {
    using Runtime::InterruptionNature;
    if (this->ptr == nullptr) {
        return;
    }

    switch (cleanup_strategy) {
    case DONT_CLEANUP:
        /* do nothing */
        break;
    case CLEANUP_WITH_FREE:
        free(this->ptr);
        break;
    case CLEANUP_WITH_DELETE:
        delete this->ptr;
        break;
    case CLEANUP_WITH_DELETE_1D_ARRAY:
        delete[] this->ptr;
        break;
    default:
        Runtime::interrupt(Runtime::UNKNOWN_CLEANUP_STRATEGY);
    }

    this->ptr = nullptr;
    this->cleanup_strategy = DONT_CLEANUP;
}

template <typename T>
__REMIXER_COERCE_INLINE auto
object(T *ptr,
       allocation_cleanup_strategy_t cleanup_strategy = CLEANUP_WITH_DELETE) -> Allocation<T> {
    return Allocation<T>(ptr, sizeof(T), cleanup_strategy);
}

template <typename T, typename... TConstructorArgs>
__REMIXER_COERCE_INLINE auto create(TConstructorArgs &&...args) -> Allocation<T> {
    return Allocation<T>(new T(std::forward<TConstructorArgs>(args)...),
                         sizeof(T), CLEANUP_WITH_DELETE);
}

template <typename T, typename TBuilder, typename... TBuilderArgs>
__REMIXER_COERCE_INLINE auto build(TBuilder &&builder,
                                   TBuilderArgs &&...args) -> Allocation<T> {
    return Allocation<T>(builder(std::forward<TBuilderArgs>(args)...),
                         sizeof(T), CLEANUP_WITH_DELETE);
}

template <typename T>
__REMIXER_COERCE_INLINE auto array_of(const std::size_t elements) -> Allocation<T> {
    return Allocation<T>(new T[elements], elements * sizeof(T),
                         CLEANUP_WITH_DELETE_1D_ARRAY);
}
} // namespace HyperstreamRemixer::Memory
