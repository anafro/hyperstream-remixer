#include "wrong-audio-buffer-cleanup-strategy-exception.h++"
#include <format>


namespace HyperstreamRemixer::Audio::Buffering::Exceptions {
    UnknownAudioBufferCleanupStrategyException::UnknownAudioBufferCleanupStrategyException(audio_buffer_cleanup_strategy_t unknown_cleanup_strategy)
        : HyperstreamException(std::format("The buffer cleanup strategy {} is unknown.", unknown_cleanup_strategy)) {}
}