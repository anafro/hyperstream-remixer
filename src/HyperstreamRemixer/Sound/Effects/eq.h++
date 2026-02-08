#pragma once
#include <array>
#include <fmt/format.h>

#include "audio-effect.h++"
#include <HyperstreamRemixer/Sound/Effects/Exceptions/eq-ascii-format-exception.h++>
#include <HyperstreamRemixer/Sugar/percents.h++>

namespace HyperstreamRemixer::Sound::Effects {
using namespace Sugar;
typedef double eq_frequency_t;
typedef double eq_gain_t;
typedef double eq_param_t;
typedef double eq_state_t;
inline constinit eq_gain_t eq_gain_min = 0_percent;
inline constinit eq_gain_t eq_gain_max = 200_percent;
constexpr std::size_t eq_bands = 16;
constexpr std::array<eq_frequency_t, eq_bands> band_frequencies = {
    20,
    35,
    50,
    80,
    120,
    300,
    500,
    800,
    1000,
    1200,
    2000,
    3500,
    5000,
    8000,
    14000,
    18000,
};
inline constinit std::array eq_band_gains_default = {
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
    100_percent,
};

struct EQBand {
    eq_frequency_t frequency;
    eq_gain_t gain;
    eq_param_t a0, a1, a2, b1, b2;
    eq_state_t z1 = 0, z2 = 0;
};

class EQ final : public AudioEffect {
  public:
    explicit EQ(std::array<eq_gain_t, eq_bands> band_gains = eq_band_gains_default);
    static auto from_bands(std::array<std::string, eq_bands + 1 /* <- for scale */> ascii_eq) -> EQ *;
    void apply(Unit<wf_amplitude_t> &audio_buffer, wf_channels_t channels, wf_sample_rate_t sample_rate) override;
    std::array<EQBand, eq_bands> bands;
};

[[nodiscard]]
static constexpr auto calculate_ascii_scale_length(const std::string &scale) -> std::size_t {
    return scale.length();
}

[[nodiscard]]
static constexpr auto calculate_ascii_band_gain(const std::size_t band_max_length, const std::string &band) -> eq_gain_t {
    if (band.length() > band_max_length) {
        throw Exceptions::EQAsciiFormatException(fmt::format("An EQ ASCII band with the length of {} exceed your scale with the length of {}", band.length(), band_max_length));
    }

    return 200_percent * (static_cast<double>(band.length()) / static_cast<double>(band_max_length));
}
} // namespace HyperstreamRemixer::Sound::Effects
