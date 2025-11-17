#define MINIMP3_IMPLEMENTATION

#include "audio.h++"

#include <iostream>
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>
#include <HyperstreamRemixer/Audio/Waveform/waveform-types.h++>
#include <HyperstreamRemixer/Audio/Buffering/Exceptions/wrong-audio-buffer-cleanup-strategy-exception.h++>

#include "HyperstreamRemixer/Performance/stopwatch.h++"


using HyperstreamRemixer::Audio::Waveform::wf_amplitude_t;

namespace HyperstreamRemixer::Audio::Buffering {
    using namespace Exceptions;

    Audio::Audio(
        const std::vector<AudioEffect*> &effects,
        const SDL_AudioFormat audio_format,
        wf_amplitude_t *buffer,
        const wf_frequency_t frequency,
        const wf_samples_t samples,
        const wf_channels_t channels,
        audio_buffer_cleanup_strategy_t buffer_cleanup_strategy
    )
        : effects(effects)
        , audio_format(audio_format)
        , buffer(buffer)
        , frequency(frequency)
        , samples(samples)
        , channels(channels)
        , buffer_cleanup_strategy(buffer_cleanup_strategy) {

        if (buffer_cleanup_strategy != AUDIO_BUFFER_DONT_CLEANUP and
            buffer_cleanup_strategy != AUDIO_BUFFER_INVOKE_FREE and
            buffer_cleanup_strategy != AUDIO_BUFFER_DELETE_ARRAY) {
            throw UnknownAudioBufferCleanupStrategyException(buffer_cleanup_strategy);
        }

        this->apply_effects();
    }


    Audio::~Audio() {
        for (const auto effect : this->effects) {
            delete effect;
        }

        this->effects.clear();

        switch (this->buffer_cleanup_strategy) {
            case AUDIO_BUFFER_DONT_CLEANUP:
                // nothing to do //
                break;
            case AUDIO_BUFFER_INVOKE_FREE:
                free(this->buffer);
                break;
            case AUDIO_BUFFER_DELETE_ARRAY:
                delete[] this->buffer;
                break;
            default: /* never reached */;
        }
    }

    void Audio::apply_effects() const {
        for (const auto effect : effects) {
            effect->apply(this->buffer, this->samples, this->channels, this->frequency);
        }
    }

    [[nodiscard]]
    Audio *Audio::from_mp3_file(const std::vector<AudioEffect*> &effects, const std::string &file_path) {
        mp3dec_t mp3dec;
        mp3dec_file_info_t mp3dec_file_info;
        mp3dec_load(&mp3dec, file_path.c_str(), &mp3dec_file_info, nullptr, nullptr);

        return new Audio(
            effects,
            AUDIO_S16,
            mp3dec_file_info.buffer,
            mp3dec_file_info.hz,
            mp3dec_file_info.samples,
            mp3dec_file_info.channels,
            AUDIO_BUFFER_INVOKE_FREE
        );
    }

    void Audio::play() const {
        SDL_AudioSpec spec{};
        spec.freq = frequency;
        spec.format = audio_format;
        spec.channels = channels;
        spec.samples = 4096;
        spec.callback = nullptr;

        SDL_AudioDeviceID dev = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
        if (!dev) {
            std::cerr << "SDL audio error: " << SDL_GetError() << std::endl;
            return;
        }

        const size_t total_bytes = samples * channels * sizeof(wf_amplitude_t);
        const size_t chunk_bytes = 4096 * channels * sizeof(wf_amplitude_t);

        size_t offset = 0;
        SDL_PauseAudioDevice(dev, 0);

        while (offset < total_bytes) {
            const size_t bytes_to_queue = std::min(chunk_bytes, total_bytes - offset);

            SDL_QueueAudio(dev, buffer + offset / sizeof(wf_amplitude_t), bytes_to_queue);

            while (SDL_GetQueuedAudioSize(dev) > 4 * chunk_bytes) {
                SDL_Delay(10);
            }

            offset += bytes_to_queue;
        }

        while (SDL_GetQueuedAudioSize(dev) > 0) {
            SDL_Delay(10);
        }

        SDL_CloseAudioDevice(dev);
    }

}
