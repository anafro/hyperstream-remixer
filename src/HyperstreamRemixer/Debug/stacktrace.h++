#pragma once
#include <sstream>
#include <stacktrace>

#include "HyperstreamRemixer/Macros/coerce-inline.h++"
namespace HyperstreamRemixer::Debug {
[[nodiscard]]
__REMIXER_COERCE_INLINE auto current_stacktrace() -> std::string {
    std::ostringstream stacktrace_buffer;
    const std::stacktrace current_stacktrace = std::stacktrace::current();
    for (auto stacktrace_entry = current_stacktrace.begin(); stacktrace_entry != current_stacktrace.end(); stacktrace_entry++) {
        stacktrace_buffer << stacktrace_entry->source_file() << ":" << stacktrace_entry->source_line() << " (" << stacktrace_entry->description() << ");\n";
    }

    return stacktrace_buffer.str();
}
} // namespace HyperstreamRemixer::Debug
