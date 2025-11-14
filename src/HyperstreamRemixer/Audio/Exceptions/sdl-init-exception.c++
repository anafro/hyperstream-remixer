#include "sdl-init-exception.h++"


namespace HyperstreamRemixer::Audio::Exceptions {
    SDLInitException::SDLInitException()
            : HyperstreamException("SDL initialization failed.") {}
}