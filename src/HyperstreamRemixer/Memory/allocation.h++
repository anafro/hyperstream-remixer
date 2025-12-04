#pragma once
#include <cstdint>

namespace HyperstreamRemixer::Memory {
    typedef uint8_t memory_cleanup_strategy_t;
    constexpr memory_cleanup_strategy_t DONT_CLEANUP = 0x00;
    constexpr memory_cleanup_strategy_t CLEANUP_WITH_FREE = 0x01;
    constexpr memory_cleanup_strategy_t CLEANUP_WITH_DELETE = 0x02;
    constexpr memory_cleanup_strategy_t CLEANUP_WITH_DELETE_1D_ARRAY = 0x03;

    template<typename T>
    class Memory final {
    public:
        Memory(T* ptr, memory_cleanup_strategy_t cleanup_strategy);
        ~Memory();
        void replace(T* new_ptr, memory_cleanup_strategy_t new_cleanup_strategy);
        memory_cleanup_strategy_t get_cleanup_strategy() const;

    private:
        T* ptr;
        memory_cleanup_strategy_t cleanup_strategy;
        void cleanup();
    };
}
