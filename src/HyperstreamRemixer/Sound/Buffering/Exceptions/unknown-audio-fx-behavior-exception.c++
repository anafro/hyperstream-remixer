#include <HyperstreamRemixer/Sound/Buffering/Exceptions/unknown-audio-fx-behavior-exception.h++>

namespace HyperstreamRemixer::Sound::Buffering::Exceptions {
    UnknownAudioFXBehaviorException::UnknownAudioFXBehaviorException()
        : HyperstreamException("Unknown audio FX behavior. See audio.h++ for allowed options.") {}
}
