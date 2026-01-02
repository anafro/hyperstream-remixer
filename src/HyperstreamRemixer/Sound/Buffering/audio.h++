#pragma once
#include <HyperstreamRemixer/Macros/coerce-inline.h++>
#include <HyperstreamRemixer/Memory/unit.h++>
#include <HyperstreamRemixer/Sound/Effects/audio-effect.h++>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

namespace HyperstreamRemixer::Sound::Buffering {
using audio_fx_behavior_t = std::byte;
constexpr audio_fx_behavior_t APPLY_FX_NOW{0x00};
constexpr audio_fx_behavior_t APPLY_FX_ON_PLAY{0x01};

using namespace Memory;
using namespace Waveform;
using namespace Effects;

class Audio final {
  public:
    Audio(
        std::vector<AudioEffect *> &&effects,
        SDL_AudioFormat audio_format,
        Unit<wf_amplitude_t> &&buffer,
        wf_frequency_t frequency,
        wf_channels_t channels,
        audio_fx_behavior_t fx_behavior = APPLY_FX_NOW);

    ~Audio();
    void play();
    static auto from_mp3_file(std::initializer_list<AudioEffect *> &&effects, const std::string &file_path, audio_fx_behavior_t fx_behavior = APPLY_FX_NOW) -> Unit<Audio>;
    void to_mp3_file(const std::string &file_path);

  protected:
    std::vector<AudioEffect *> effects;
    SDL_AudioFormat audio_format;
    Unit<wf_amplitude_t> original_buffer;
    Unit<wf_amplitude_t> buffer_with_effects;
    wf_frequency_t frequency;
    wf_channels_t channels;
    audio_fx_behavior_t fx_behavior;

  private:
    auto apply_effects(const Unit<wf_amplitude_t> &buffer) const -> Unit<wf_amplitude_t>;
    [[nodiscard]] __REMIXER_COERCE_INLINE auto get_samples() const -> wf_samples_t;
};
} // namespace HyperstreamRemixer::Sound::Buffering
