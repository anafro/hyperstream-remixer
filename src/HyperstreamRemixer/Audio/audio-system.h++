#pragma once
#include "Exceptions/already-initialized-exception.h++"

using HyperstreamRemixer::Audio::Exceptions::AlreadyInitializedException;

namespace HyperstreamRemixer::Audio {
    static bool remixer_is_initialized = false;

    static void initialize_remixer() {
        if (remixer_is_initialized) {
            throw AlreadyInitializedException();
        }

        remixer_is_initialized = true;
        std::cout << "(initialize_remixer is not implemented yet)" << std::endl;
    }
}
