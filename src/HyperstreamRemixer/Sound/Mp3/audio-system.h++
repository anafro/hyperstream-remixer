#pragma once
#include "../Exceptions/already-initialized-exception.h++"
#include "../Exceptions/not-initialized-exception.h++"
#include "../Exceptions/sdl-init-exception.h++"
#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <fmt/format.h>

using HyperstreamRemixer::Sound::Exceptions::AlreadyInitializedException;
using HyperstreamRemixer::Sound::Exceptions::NotInitializedException;
using HyperstreamRemixer::Sound::Exceptions::SDLInitException;

namespace HyperstreamRemixer::Sound {
static bool remixer_is_initialized = false;

static void initialize_remixer() {
    if (remixer_is_initialized) {
        throw AlreadyInitializedException();
    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        throw SDLInitException(fmt::format("Initializing SDL Audio failed: {}.", SDL_GetError()));
    }

    remixer_is_initialized = true;
}

static void clean_up_remixer() {
    if (!remixer_is_initialized) {
        throw NotInitializedException();
    }

    SDL_Quit();
    remixer_is_initialized = false;
}
} // namespace HyperstreamRemixer::Sound
