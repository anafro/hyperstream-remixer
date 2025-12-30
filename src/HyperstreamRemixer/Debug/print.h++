#pragma once
#include <HyperstreamRemixer/Debug/stacktrace.h++>
#include <HyperstreamRemixer/Environment/configuration.h++>
#include <print>

#ifdef REMIXER_$_PRINTS_ON
#define $(value) ([&]() -> auto { std::println("{} = {}", #value, (value)); return (value); })
#define LOG(value) \
    std::println("{} at {}", (value), __FUNCTION__);
#else
#define $(value) (value)
#define LOG(value)
#endif
