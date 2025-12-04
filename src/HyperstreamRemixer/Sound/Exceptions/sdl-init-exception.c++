#include "sdl-init-exception.h++"


namespace HyperstreamRemixer::Audio::Exceptions {
    SDLInitException::SDLInitException(const std::string& message)
            : HyperstreamException(message) {}
}