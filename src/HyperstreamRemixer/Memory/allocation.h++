#pragma once
#include <HyperstreamRemixer/Memory/Exceptions/unknown-memory-cleanup-strategy-exception.h++>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <format>
#include <type_traits>

#include <HyperstreamRemixer/Macros/coerce-inline.h++>
#include <HyperstreamRemixer/Memory/Exceptions/allocation-clone-copy-exception.h++>

#include "HyperstreamRemixer/Debug/print.h++"

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
    Allocation(const Allocation &) = delete;
    Allocation(Allocation &&) noexcept;
    Allocation(T *ptr, std::size_t size,
               allocation_cleanup_strategy_t cleanup_strategy);
    ~Allocation();
    void replace(T *new_ptr, std::size_t new_size,
                 allocation_cleanup_strategy_t new_cleanup_strategy);
    [[nodiscard]] allocation_cleanup_strategy_t get_cleanup_strategy() const;
    Allocation &operator=(const Allocation &) = delete;
    Allocation &operator=(Allocation &&that);
    T *raw();
    const T *raw() const;
    T *operator->();
    const T *operator->() const;
    T *operator*();
    const T *operator*() const;
    Allocation create_view();
    Allocation create_copy() const;
    [[nodiscard]] std::size_t get_size() const;
    template <typename S = std::size_t>
    [[nodiscard]] S get_length() const;
    static Allocation null();

  private:
    T *ptr;
    std::size_t size;
    allocation_cleanup_strategy_t cleanup_strategy;
    void cleanup();
};

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
allocation_cleanup_strategy_t Allocation<T>::get_cleanup_strategy() const {
    return this->cleanup_strategy;
}

template <typename T>
Allocation<T> &Allocation<T>::operator=(Allocation<T> &&that) {
    this->cleanup();
    this->ptr = that.ptr;
    this->size = that.size;
    this->cleanup_strategy = that.cleanup_strategy;
    that.ptr = nullptr;
    that.cleanup_strategy = DONT_CLEANUP;

    return *this;
}

template <typename T>
T *Allocation<T>::raw() { return this->ptr; }

template <typename T>
const T *Allocation<T>::raw() const { return this->ptr; }

template <typename T>
T *Allocation<T>::operator->() { return raw(); }

template <typename T>
const T *Allocation<T>::operator->() const {
    return raw();
}

template <typename T>
T *Allocation<T>::operator*() { return raw(); }

template <typename T>
const T *Allocation<T>::operator*() const {
    return raw();
}

template <typename T>
Allocation<T> Allocation<T>::create_view() {
    return Allocation(this->ptr, this->size, DONT_CLEANUP);
}

template <typename T>
Allocation<T> Allocation<T>::create_copy() const {
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
std::size_t Allocation<T>::get_size() const {
    return this->size;
}

template <typename T>
template <typename S>
S Allocation<T>::get_length() const {
    return S(this->size / sizeof(T));
}

template <typename T>
Allocation<T> Allocation<T>::null() {
    return Allocation(nullptr, sizeof(std::nullptr_t), DONT_CLEANUP);
}

template <typename T>
void Allocation<T>::cleanup() {
    if (this->ptr == nullptr) {
        return;
    }

    $(typeid(T).name());

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
        throw UnknownMemoryCleanupStrategyException();
    }

    this->ptr = nullptr;
    this->cleanup_strategy = DONT_CLEANUP;
}

template <typename T>
__REMIXER_COERCE_INLINE Allocation<T>
object(T *ptr,
       allocation_cleanup_strategy_t cleanup_strategy = CLEANUP_WITH_DELETE) {
    return Allocation<T>(ptr, sizeof(T), cleanup_strategy);
}

template <typename T, typename... TConstructorArgs>
__REMIXER_COERCE_INLINE Allocation<T> create(TConstructorArgs &&...args) {
    return Allocation<T>(new T(std::forward<TConstructorArgs>(args)...),
                         sizeof(T), CLEANUP_WITH_DELETE);
}

template <typename T, typename TBuilder, typename... TBuilderArgs>
__REMIXER_COERCE_INLINE Allocation<T> build(TBuilder &&builder,
                                            TBuilderArgs &&...args) {
    return Allocation<T>(builder(std::forward<TBuilderArgs>(args)...),
                         sizeof(T), CLEANUP_WITH_DELETE);
}

template <typename T>
__REMIXER_COERCE_INLINE Allocation<T> array_of(const std::size_t elements) {
    return Allocation<T>(new T[elements], elements * sizeof(T),
                         CLEANUP_WITH_DELETE_1D_ARRAY);
}
} // namespace HyperstreamRemixer::Memory
