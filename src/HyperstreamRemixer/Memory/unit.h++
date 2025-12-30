#pragma once
#include "HyperstreamRemixer/Debug/print.h++"
#include "HyperstreamRemixer/Multithreading/optional-lock.h++"
#include <HyperstreamRemixer/Macros/coerce-inline.h++>
#include <HyperstreamRemixer/Memory/Exceptions/allocation-clone-copy-exception.h++>
#include <HyperstreamRemixer/Memory/Exceptions/unknown-memory-cleanup-strategy-exception.h++>
#include <HyperstreamRemixer/Runtime/interrupt.h++>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <mutex>
#include <type_traits>
#include <utility>
#include <vector>

namespace HyperstreamRemixer::Memory {
using namespace Exceptions;
using namespace Multithreading;

enum PointerType : uint8_t {
    C = 0x01,
    C_ARRAY = 0x02,
    CXX = 0x03,
    CXX_ARRAY = 0x04,
    NULL_POINTER = 0x05,
    DANGLED = 0x06,
    WILD = 0x07,
};

[[nodiscard]]
inline auto pointer_type_to_string(PointerType pointer_type) -> std::string {
    switch (pointer_type) {
    case C:
        return "C";
    case C_ARRAY:
        return "C_ARRAY";
    case CXX:
        return "CXX";
    case CXX_ARRAY:
        return "CXX_ARRAY";
    case NULL_POINTER:
        return "NULL_POINTER";
    case DANGLED:
        return "DANGLED";
    case WILD:
        return "WILD";
    }
}

template <typename T>
class Unit final {
    std::string name;
    PointerType pointer_type = WILD;
    T *memory_pointer;
    std::size_t memory_size;
    mutable std::recursive_timed_mutex *memory_mutex;
    mutable Unit *holder;
    mutable std::vector<Unit *> viewers;

    Unit(PointerType pointer_type, T *memory_pointer, std::size_t memory_size, std::recursive_timed_mutex *memory_mutex, Unit<T> *holder, const std::string &name = "");
    [[nodiscard]] auto has_memory_mutex() -> bool;
    void free_memory();
    void assign(PointerType pointer_type, T *memory_pointer, std::size_t memory_size);
    void add_viewer(Unit *view) const;
    void remove_viewer(Unit *view) const;

  public:
    Unit();
    Unit(Unit &that);
    Unit(const Unit &that);
    Unit(Unit &&that) noexcept;
    ~Unit();
    [[nodiscard]] static auto null() noexcept -> Unit<T>;
    [[nodiscard]] static auto wrap_object(PointerType pointer_type, T *memory_pointer, const std::string &name = "") -> Unit<T>;
    [[nodiscard]] static auto wrap_array(PointerType pointer_type, T *memory_pointer, std::size_t length, const std::string &name = "") -> Unit<T>;
    auto operator=(const Unit<T> &) -> Unit<T> & = delete;
    auto operator=(Unit<T> &&that) noexcept -> Unit<T> &;
    [[nodiscard]] auto operator->() -> T *;
    [[nodiscard]] auto operator->() const -> const T *;
    [[nodiscard]] auto operator*() -> T *;
    [[nodiscard]] auto operator*() const -> const T *;
    [[nodiscard]] auto operator[](std::size_t index) -> T &;
    [[nodiscard]] auto operator[](std::size_t index) const -> const T &;
    [[nodiscard]] auto size() const -> std::size_t;
    [[nodiscard]] auto length() const noexcept -> std::size_t;
    [[nodiscard]] auto copy() const noexcept -> Unit;
    [[nodiscard]] auto views() const noexcept -> bool;
    [[nodiscard]] auto holds() const noexcept -> bool;
    [[nodiscard]] auto is_null() const noexcept -> bool;
    [[nodiscard]] auto non_null() const noexcept -> bool;
    [[nodiscard]] auto is_object() const noexcept -> bool;
    [[nodiscard]] auto is_array() const noexcept -> bool;
    [[nodiscard]] auto points_to(const void *memory_pointer) const -> bool;
    [[nodiscard]] auto is(PointerType pointer_type) const -> bool;
    [[nodiscard]] auto takes(std::size_t memory_size) const -> bool;
    [[nodiscard]] auto count_references() const -> std::size_t;
    [[nodiscard]] auto to_string() const -> std::string;
    void replace(PointerType replacement_pointer_type, T *memory_replacement_pointer, std::size_t replacement_size);
    void nullify();
};

template <typename T>
Unit<T>::Unit() : Unit(NULL_POINTER, nullptr, sizeof(std::nullptr_t), nullptr, nullptr) {
}

template <typename T>
Unit<T>::Unit(PointerType pointer_type, T *memory_pointer, std::size_t memory_size, std::recursive_timed_mutex *memory_mutex, Unit *holder, const std::string &name)
    : pointer_type(pointer_type), memory_pointer(memory_pointer), memory_size(memory_size), memory_mutex(memory_mutex), holder(holder), name(name) {
}

template <typename T>
[[nodiscard]] auto Unit<T>::has_memory_mutex() -> bool {
    auto lock = OptionalLock{memory_mutex};
    return memory_mutex != nullptr;
}

template <typename T>
void Unit<T>::free_memory() {
    if (pointer_type == C || pointer_type == C_ARRAY) {
        free(this->memory_pointer);
    }

    if (pointer_type == CXX) {
        delete memory_pointer;
    }

    if (pointer_type == CXX_ARRAY) {
        delete[] memory_pointer;
    }
    delete memory_mutex;
    pointer_type = DANGLED;

    nullify();
}

template <typename T>
void Unit<T>::assign(PointerType pointer_type, T *memory_pointer, std::size_t memory_size) {
    auto lock = OptionalLock{memory_mutex};
    this->pointer_type = pointer_type;
    this->memory_pointer = memory_pointer;
    this->memory_size = memory_size;
}

template <typename T>
void Unit<T>::add_viewer(Unit *viewer) const {
    auto lock = OptionalLock{memory_mutex};
    viewers.push_back(viewer);
}

template <typename T>
void Unit<T>::remove_viewer(Unit *viewer) const {
    {
        if (views()) {
            interrupt(Runtime::REMOVE_VIEWER_FROM_VIEWER);
        }
    }
    auto lock = OptionalLock{memory_mutex};
    viewers.erase(std::remove(viewers.begin(), viewers.end(), viewer), viewers.end());
}

template <typename T>
Unit<T>::Unit(Unit &that)
    : Unit(that.pointer_type, that.memory_pointer, that.memory_size, that.memory_mutex, that.holds() ? &that : that.holder) {
    that.add_viewer(this);
}

template <typename T>
Unit<T>::Unit(const Unit &that)
    : Unit(that.pointer_type, that.memory_pointer, that.memory_size, that.memory_mutex, &that) {
    that.add_viewer(this);
}

template <typename T>
Unit<T>::Unit(Unit &&that) noexcept
    : Unit(that.pointer_type, that.memory_pointer, that.memory_size, that.memory_mutex, that.holder) {
    that.nullify();
}

template <typename T>
Unit<T>::~Unit() {
    if (holds()) {
        auto lock = OptionalLock{memory_mutex};

        for (auto *viewer : viewers) {
            viewer->nullify();
        }

        viewers.clear();
        this->free_memory();
    }

    if (views()) {
        auto lock = OptionalLock{memory_mutex};
        holder->remove_viewer(this);
    }
}

template <typename T>
[[nodiscard]] auto Unit<T>::null() noexcept -> Unit {
    return Unit(NULL_POINTER, nullptr, sizeof(std::nullptr_t), nullptr, nullptr);
}

template <typename T>
[[nodiscard]] auto Unit<T>::wrap_object(PointerType pointer_type, T *memory_pointer, const std::string &name) -> Unit {
    return Unit(pointer_type, memory_pointer, sizeof(T), new std::recursive_timed_mutex{}, nullptr, name);
}

template <typename T>
[[nodiscard]] auto Unit<T>::wrap_array(PointerType pointer_type, T *memory_pointer, std::size_t array_length, const std::string &name) -> Unit {
    return Unit(pointer_type, memory_pointer, sizeof(T) * array_length, new std::recursive_timed_mutex{}, nullptr);
}

template <typename T>
auto Unit<T>::operator=(Unit &&that) noexcept -> Unit & {
    if (this != &that) {
        {
            this->free_memory();
        }

        {
            auto lock = OptionalLock{memory_mutex};
            pointer_type = that.pointer_type;
            memory_pointer = std::move(that.memory_pointer);
            memory_size = that.memory_size;
            memory_mutex = that.memory_mutex;
            holder = std::move(that.holder);
            viewers = std::move(that.viewers);
        }

        that.nullify();
    }

    return *this;
}

template <typename T>
[[nodiscard]] auto Unit<T>::operator->() -> T * {
    auto lock = OptionalLock{memory_mutex};
    return memory_pointer;
}

template <typename T>
[[nodiscard]] auto Unit<T>::operator->() const -> const T * {
    auto lock = OptionalLock{memory_mutex};
    return memory_pointer;
}

template <typename T>
[[nodiscard]] auto Unit<T>::operator*() -> T * {
    auto lock = OptionalLock{memory_mutex};
    return memory_pointer;
}

template <typename T>
[[nodiscard]] auto Unit<T>::operator*() const -> const T * {
    auto lock = OptionalLock{memory_mutex};
    return memory_pointer;
}

template <typename T>
[[nodiscard]] auto Unit<T>::operator[](std::size_t index) -> T & {
    auto lock = OptionalLock{memory_mutex};
    return memory_pointer[index];
}

template <typename T>
[[nodiscard]] auto Unit<T>::operator[](std::size_t index) const -> const T & {
    auto lock = OptionalLock{memory_mutex};
    return memory_pointer[index];
}

template <typename T>
[[nodiscard]] auto Unit<T>::size() const -> std::size_t {
    auto lock = OptionalLock{memory_mutex};
    return memory_size;
}

template <typename T>
[[nodiscard]] auto Unit<T>::length() const noexcept -> std::size_t {
    auto lock = OptionalLock{memory_mutex};
    return memory_size / sizeof(T);
}

template <typename T>
[[nodiscard]] auto Unit<T>::copy() const noexcept -> Unit<T> {
    auto lock = OptionalLock{memory_mutex};
    T *memory_copy_pointer;

    switch (pointer_type) {
    case C:
    case C_ARRAY:
        memory_copy_pointer = reinterpret_cast<T *>(malloc(memory_size));
        std::memcpy(memory_copy_pointer, memory_pointer, memory_size);
        break;
    case CXX:
        if constexpr (not std::is_trivially_copy_constructible_v<T>) {
            interrupt(Runtime::COPY_OF_NON_TRIVIAL_CXX_MEMORY);
        }

        memory_copy_pointer = new T(*memory_pointer);
        break;
    case CXX_ARRAY:
        memory_copy_pointer = new T[this->length()];
        std::memcpy(memory_copy_pointer, memory_pointer, memory_size);
        break;
    case NULL_POINTER:
        return null();
    case DANGLED:
        interrupt(Runtime::COPY_OF_DANGLED_MEMORY);
    case WILD:
        interrupt(Runtime::COPY_OF_WILD_MEMORY);
    default:
        interrupt(Runtime::COPY_OF_MEMORY_WITH_UNKNOWN_TYPE);
    }

    return Unit(pointer_type, memory_copy_pointer, memory_size, new std::recursive_timed_mutex{}, nullptr);
}

template <typename T>
[[nodiscard]] auto Unit<T>::views() const noexcept -> bool {
    if (is_null()) {
        return false;
    }

    auto lock = OptionalLock{memory_mutex};
    return holder != nullptr;
}

template <typename T>
[[nodiscard]] auto Unit<T>::holds() const noexcept -> bool {
    if (is_null()) {
        return false;
    }

    return not views();
}

template <typename T>
[[nodiscard]] auto Unit<T>::is_null() const noexcept -> bool {
    auto lock = OptionalLock{memory_mutex};
    return memory_pointer == nullptr;
}

template <typename T>
[[nodiscard]] auto Unit<T>::non_null() const noexcept -> bool {
    return not is_null();
}

template <typename T>
[[nodiscard]] auto Unit<T>::is_object() const noexcept -> bool {
    if (is_null()) {
        return false;
    }

    auto lock = OptionalLock{memory_mutex};
    return pointer_type == C or pointer_type == CXX;
}

template <typename T>
[[nodiscard]] auto Unit<T>::is_array() const noexcept -> bool {
    if (is_null()) {
        return false;
    }

    auto lock = OptionalLock{memory_mutex};
    return pointer_type == C_ARRAY or pointer_type == CXX_ARRAY;
}

template <typename T>
[[nodiscard]] auto Unit<T>::points_to(const void *memory_pointer) const -> bool {
    auto lock = OptionalLock{memory_mutex};
    return this->memory_pointer == memory_pointer;
}

template <typename T>
[[nodiscard]] auto Unit<T>::is(PointerType pointer_type) const -> bool {
    auto lock = OptionalLock{memory_mutex};
    return this->pointer_type == pointer_type;
}

template <typename T>
[[nodiscard]] auto Unit<T>::takes(std::size_t memory_size) const -> bool {
    auto lock = OptionalLock{memory_mutex};
    return this->memory_size == memory_size;
}

template <typename T>
[[nodiscard]] auto Unit<T>::count_references() const -> std::size_t {
    if (views()) {
        return holder->count_references();
    }

    if (holds()) {
        auto lock = OptionalLock{memory_mutex};
        return 1 /* (for holder) */ + viewers.size();
    }

    return 1;
}

template <typename T>
void Unit<T>::replace(PointerType replacement_pointer_type, T *memory_replacement_pointer, std::size_t memory_replacement_size) {
    if (holds()) {
        {
            free_memory();
        }

        {
            auto lock = OptionalLock{memory_mutex};
            for (auto *viewer : viewers) {
                viewer->assign(replacement_pointer_type, memory_replacement_pointer, memory_replacement_size);
            }
        }

        {
            assign(replacement_pointer_type, memory_replacement_pointer, memory_replacement_size);
        }
    }

    if (views()) {
        holder->replace(replacement_pointer_type, memory_replacement_pointer, memory_replacement_size);
    }
}

template <typename T>
[[nodiscard]] auto Unit<T>::to_string() const -> std::string {
    return std::format(
        "<{}{} {} {} BYTE UNIT<{}> @ {:x} ({} UNITS FOR MEMORY)>",
        name == "" ? "" : name + ": ",
        pointer_type_to_string(pointer_type),
        holds() ? "HOLDER" : (views() ? "VIEWER" : "ORPHAN"),
        size(),
        typeid(T).name(),
        reinterpret_cast<std::uintptr_t>(memory_pointer),
        count_references());
}

template <typename T>
void Unit<T>::nullify() {
    pointer_type = NULL_POINTER;
    memory_pointer = nullptr;
    memory_size = sizeof(std::nullptr_t);
    memory_mutex = nullptr;
    holder = nullptr;
}
} // namespace HyperstreamRemixer::Memory
