#pragma once
#include "audio-effect.h++"
#include <HyperstreamRemixer/Debug/print.h++>
#include <chrono>
#include <cstring>

namespace HyperstreamRemixer::Sound::Effects {
using namespace std::chrono_literals;

inline constinit std::chrono::seconds fx_remainder_default = 4s;

class Remainder final : public AudioEffect {
  public:
    template <class Rep, class Period>
    explicit Remainder(std::chrono::duration<Rep, Period> remainder_duration = fx_remainder_default)
        : remainder_duration(std::chrono::duration<double>(remainder_duration)) {}

    void apply(Allocation<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;

  private:
    std::chrono::duration<double> remainder_duration;
    [[nodiscard]] auto calculate_remainder_length(wf_channels_t channels, wf_sample_rate_t sample_rate) const -> wf_samples_t;
    REMIXER_VISUAL_DEBUGGER_FRIEND_DEFINITION();
};

inline void Remainder::apply(Allocation<wf_amplitude_t> &audio_buffer, const wf_channels_t channels, const wf_sample_rate_t sample_rate) {
    const auto old_audio_length = audio_buffer.get_length<wf_samples_t>();
    const auto new_audio_length = old_audio_length + calculate_remainder_length(channels, sample_rate);
    auto *new_audio_buffer = new wf_amplitude_t[new_audio_length];

    std::memcpy(new_audio_buffer, audio_buffer.raw(), old_audio_length * sizeof(wf_amplitude_t));

    audio_buffer.replace(new_audio_buffer, new_audio_length, CLEANUP_WITH_DELETE_1D_ARRAY);
}

inline auto Remainder::calculate_remainder_length(const wf_channels_t channels, const wf_sample_rate_t sample_rate) const -> wf_samples_t {
    const double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(remainder_duration).count();
    return static_cast<wf_samples_t>(std::ceil(seconds * sample_rate));
}
} // namespace HyperstreamRemixer::Sound::Effects
