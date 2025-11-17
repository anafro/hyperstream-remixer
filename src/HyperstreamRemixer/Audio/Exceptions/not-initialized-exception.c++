#include "not-initialized-exception.h++"


namespace HyperstreamRemixer::Audio::Exceptions {
    NotInitializedException::NotInitializedException()
        : HyperstreamException("Hyperstream Remixer is not initialized. Add init call.") {}
}