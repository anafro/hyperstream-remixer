#include "unknown-audio-fx-behavior-exception.h++"

namespace HyperstreamRemixer::Audio::Buffering::Exceptions {
    UnknownAudioFXBehaviorException::UnknownAudioFXBehaviorException()
        : HyperstreamException("Unknown audio FX behavior. See audio.h++ for allowed options.") {}
}
