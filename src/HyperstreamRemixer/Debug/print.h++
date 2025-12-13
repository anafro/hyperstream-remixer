#pragma once
#include <HyperstreamRemixer/Environment/configuration.h++>

#ifdef REMIXER_$_PRINTS_ON
    #include <iostream>
    #define $(value) ([&](){ std::cout << "Debug [" << #value << " = " << value << "]\n"; return (value); })()
#else
    #define $(value) (value)
#endif
