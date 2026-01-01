#pragma once
#include "HyperstreamRemixer/Environment/configuration.h++"
#include "HyperstreamRemixer/Macros/coerce-inline.h++"
#include "HyperstreamRemixer/Macros/expression-stringify.h++"
#include <HyperstreamRemixer/Maps/safe-access.h++>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <print>

namespace HyperstreamRemixer::Runtime {
using namespace Maps;
using interruption_nature_ordinal_t = uint_fast8_t;
enum InterruptionNature : interruption_nature_ordinal_t {
    COPY_OF_NON_TRIVIAL_CXX_MEMORY = 0x01,
    COPY_OF_DANGLED_MEMORY = 0x02,
    COPY_OF_WILD_MEMORY = 0x03,
    COPY_OF_MEMORY_WITH_UNKNOWN_TYPE = 0x04,
    REMOVE_VIEWER_FROM_VIEWER = 0x05,

};

[[noreturn]]
__REMIXER_COERCE_INLINE void interrupt(InterruptionNature interruption_nature) {
    const std::map<InterruptionNature, std::string> interruption_messages{
        {COPY_OF_NON_TRIVIAL_CXX_MEMORY, "Copy of non-trivial C++ memory"},
        {COPY_OF_DANGLED_MEMORY, "Copy of memory pointed by a dangling pointer"},
        {COPY_OF_WILD_MEMORY, "Copy of memory pointed by a wild pointer"},
        {COPY_OF_MEMORY_WITH_UNKNOWN_TYPE, "Copy of a memory pointed by a pointer with unknown type"},
        {REMOVE_VIEWER_FROM_VIEWER, "Remove a viewer unit from another viewer unit"}};

    const auto message = map_get(interruption_messages, interruption_nature).value_or("(no message)");

#ifdef REMIXER_EXIT_ON_INTERRUPTION_ON
    std::println(stderr, "Remixer is interrupted by an error: {} (interruption nature {:#x})", message, static_cast<interruption_nature_ordinal_t>(interruption_nature));
    std::println(stderr, "To disable exiting on interruption for debugging purposes, comment the {} macros in Environment/configuration.h++", STR_EXPR(REMIXER_EXIT_ON_INTERRUPTION_ON));
    std::exit(interruption_nature);
#else
    std::println(stderr, "Remixer came across with an interruption condition: {} (interruption nature {:#x})", message, static_cast<interruption_nature_ordinal_t>(interruption_nature));
    std::println(stderr, "Yet Remixer HAS NOT EXITED, since exiting was disabled.");
    std::println(stderr, "To enable back exiting on interruption, uncomment the {} macros in Environment/configuration.h++", STR_EXPR(REMIXER_EXIT_ON_INTERRUPTION_ON));
#endif
}
} // namespace HyperstreamRemixer::Runtime
