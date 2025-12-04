#pragma once
#include <HyperstreamRemixer/Exceptions/hyperstream-exception.h++>

namespace HyperstreamRemixer::Audio::Buffering::Exceptions {
    class UnknownAudioFXBehaviorException final : public HyperstreamRemixer::Exceptions::HyperstreamException {
    public:
        explicit UnknownAudioFXBehaviorException();
    };
}
