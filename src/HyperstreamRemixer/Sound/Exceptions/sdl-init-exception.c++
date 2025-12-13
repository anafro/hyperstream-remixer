#include "sdl-init-exception.h++"


namespace HyperstreamRemixer::Sound::Exceptions {
    SDLInitException::SDLInitException(const std::string& message)
            : HyperstreamException(message) {}
}