#include "already-initialized-exception.h++"


namespace HyperstreamRemixer::Audio::Exceptions {
    AlreadyInitializedException::AlreadyInitializedException()
        : HyperstreamException("Hyperstream Remixer can't be initialized twice. Search for duplicated init calls.") {}
}