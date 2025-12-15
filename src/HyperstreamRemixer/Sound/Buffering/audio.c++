#define MINIMP3_IMPLEMENTATION

#include "audio.h++"

#include <HyperstreamRemixer/Sound/Waveform/waveform-types.h++>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>

#include "Exceptions/audio-file-not-found-exception.h++"
#include "Exceptions/unknown-audio-fx-behavior-exception.h++"
#include "HyperstreamRemixer/Performance/stopwatch.h++"

namespace HyperstreamRemixer::Sound::Buffering {
using namespace Exceptions;

Audio::Audio(
    std::vector<AudioEffect *> &&effects,
    const SDL_AudioFormat audio_format,
    Allocation<wf_amplitude_t> &&buffer,
    const wf_frequency_t frequency,
    const wf_channels_t channels,
    const audio_fx_behavior_t fx_behavior)
    : effects(std::move(effects)), audio_format(audio_format), original_buffer(std::move(buffer)), buffer_with_effects(Allocation<wf_amplitude_t>::null()), frequency(frequency), channels(channels), fx_behavior(fx_behavior) {
    switch (fx_behavior) {
    case APPLY_FX_NOW:
        this->buffer_with_effects = std::move(this->apply_effects(this->original_buffer));
        break;

    case APPLY_FX_ON_PLAY:
        // Buffers will be initialized on play
        break;

    default:
        throw UnknownAudioFXBehaviorException();
    }
}

Audio::~Audio() {
    for (const auto effect : this->effects) {
        delete effect;
    }

    this->effects.clear();
}

void Audio::play() {
    if (fx_behavior == APPLY_FX_ON_PLAY) {
        buffer_with_effects = apply_effects(original_buffer);
    }

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

    const std::size_t total_bytes = get_samples() * sizeof(wf_amplitude_t);
    const std::size_t chunk_bytes = spec.samples * channels * sizeof(wf_amplitude_t);

    std::size_t offset = 0;
    SDL_PauseAudioDevice(dev, 0);

    while (offset < total_bytes) {
        const std::size_t bytes_to_queue = std::min(chunk_bytes, total_bytes - offset);

        SDL_QueueAudio(dev, this->buffer_with_effects.raw() + offset / sizeof(wf_amplitude_t), bytes_to_queue);

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

[[nodiscard]]
auto Audio::from_mp3_file(std::initializer_list<AudioEffect *> &&effects, const std::string &file_path, const audio_fx_behavior_t fx_behavior) -> Audio * {
    if (not std::filesystem::exists(file_path)) {
        throw AudioFileNotFoundException(file_path);
    }

    mp3dec_t mp3dec;
    mp3dec_file_info_t mp3dec_file_info;
    mp3dec_load(&mp3dec, file_path.c_str(), &mp3dec_file_info, nullptr, nullptr);

    static_assert(
        sizeof(*mp3dec_file_info.buffer) == sizeof(wf_amplitude_t),
        "The Hyperstream's sample type size mismatch the MP3 library sample type size. This probably means "
        "that your OS architecture is not supported. We apologise for that...");

    return new Audio(
        effects,
        AUDIO_S16,
        Allocation(mp3dec_file_info.buffer, mp3dec_file_info.samples * mp3dec_file_info.channels, CLEANUP_WITH_FREE),
        mp3dec_file_info.hz,
        mp3dec_file_info.channels,
        fx_behavior);
}

auto Audio::apply_effects(const Allocation<wf_amplitude_t> &buffer) const -> Allocation<wf_amplitude_t> {
    auto buffer_copy = buffer.create_copy();

    for (const auto effect : effects) {
        effect->apply(buffer_copy, this->channels, this->frequency);
    }

    return buffer_copy;
}

auto Audio::get_samples() const -> wf_samples_t {
    return this->buffer_with_effects.get_length<>();
}
} // namespace HyperstreamRemixer::Sound::Buffering
