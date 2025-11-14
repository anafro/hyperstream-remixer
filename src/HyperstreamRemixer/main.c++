#include <iostream>
#include <SDL3/SDL.h>

#include "Audio/audio-system.h++"
#include "Audio/Exceptions/sdl-init-exception.h++"


using HyperstreamRemixer::Audio::initialize_remixer;
using HyperstreamRemixer::Exceptions::HyperstreamException;

int main(int argc, char* argv[]) {
    try {
        initialize_remixer();

        return EXIT_SUCCESS;
    } catch (const HyperstreamException& exception) {
        std::cerr << "Hyperstream Remixer crashed:" << std::endl;
        std::cerr << "See exception to find cause:" << std::endl;
        std::cerr << exception.what() << std::endl;

        return EXIT_FAILURE;
    }
}
