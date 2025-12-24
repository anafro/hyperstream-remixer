#include "HyperstreamRemixer/Macros/coerce-inline.h++"
#include <HyperstreamRemixer/Maps/safe-access.h++>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>

namespace HyperstreamRemixer::Runtime {
using namespace Maps;
using interruption_nature_ordinal_t = uint_fast8_t;
enum InterruptionNature : interruption_nature_ordinal_t {
    OK = 0x00,
    UNKNOWN_CLEANUP_STRATEGY = 0x01,
};
__REMIXER_COERCE_INLINE void interrupt(InterruptionNature interruption_nature) {
    const std::map<InterruptionNature, std::string> interruption_messages{
        {OK, "Successfully finished"},
        {UNKNOWN_CLEANUP_STRATEGY, "An unknown allocation cleanup strategy is used"}};

    const auto message = map_get(interruption_messages, interruption_nature).value_or(std::format("Interrupted with code {}", interruption_nature, static_cast<interruption_nature_ordinal_t>(interruption_nature));
    std::cerr << message << '\n';
    std::exit(interruption_nature);
}
} // namespace HyperstreamRemixer::Runtime
