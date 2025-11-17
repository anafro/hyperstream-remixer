#pragma once
#include "HyperstreamRemixer/Audio/Buffering/audio.h++"
#include "HyperstreamRemixer/Exceptions/hyperstream-exception.h++"

namespace HyperstreamRemixer::Audio::Buffering::Exceptions {
    using namespace HyperstreamRemixer::Exceptions;

    class UnknownAudioBufferCleanupStrategyException final : public HyperstreamException {
    public:
        explicit UnknownAudioBufferCleanupStrategyException(audio_buffer_cleanup_strategy_t unknown_cleanup_strategy);
    };
}
