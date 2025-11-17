#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "Exceptions/already-initialized-exception.h++"
#include "Exceptions/not-initialized-exception.h++"
#include "Exceptions/sdl-init-exception.h++"

using HyperstreamRemixer::Audio::Exceptions::AlreadyInitializedException;
using HyperstreamRemixer::Audio::Exceptions::NotInitializedException;
using HyperstreamRemixer::Audio::Exceptions::SDLInitException;

namespace HyperstreamRemixer::Audio {
    static bool remixer_is_initialized = false;

    static void initialize_remixer() {
        if (remixer_is_initialized) {
            throw AlreadyInitializedException();
        }

        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            throw SDLInitException("Initializing SDL Audio failed.");
        }

        Mix_Init(MIX_INIT_MP3);
        remixer_is_initialized = true;
    }

    static void clean_up_remixer() {
        if (! remixer_is_initialized) {
            throw NotInitializedException();
        }

        Mix_Quit();
        SDL_Quit();
        remixer_is_initialized = false;
    }
}
