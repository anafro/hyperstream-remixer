#pragma once
#include <HyperstreamRemixer/Environment/configuration.h++>

#ifdef REMIXER_$_PRINTS_ON
#define $(value) ([&]() -> auto { std::cout << "Debug [" << #value << " = " << value << "]\n"; return (value); })()
#else
#define $(value) (value)
#endif
