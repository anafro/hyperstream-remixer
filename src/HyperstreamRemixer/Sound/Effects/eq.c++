#include "eq.h++"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <vector>

#include "HyperstreamRemixer/Debug/print.h++"
#include "HyperstreamRemixer/Sound/Adapters/freeverb-adapters.h++"

namespace HyperstreamRemixer::Sound::Effects {
EQ::EQ(std::array<eq_gain_t, eq_bands> band_gains) {
    std::transform(band_gains.begin(), band_gains.end(), band_frequencies.begin(), this->bands.begin(), [](const auto gain, const auto frequency) {
        return EQBand{
            .frequency = frequency,
            .gain = gain,
        };
    });
}

EQ *EQ::from_bands(std::array<std::string, eq_bands + 1 /* <- for scale */> ascii_eq) {
    const auto &ascii_scale = ascii_eq[0];
    const std::size_t max_band_size = calculate_ascii_scale_length(ascii_scale);
    std::array<eq_gain_t, eq_bands> band_gains;
    std::transform(ascii_eq.begin() + 1 /* <- for scale */, ascii_eq.end(), band_gains.begin(), [max_band_size](const std::string &band) {
        return calculate_ascii_band_gain(max_band_size, band);
    });

    return new EQ(band_gains);
}

void EQ::apply(Allocation<wf_amplitude_t> &audio_buffer,
               const wf_channels_t channels,
               const wf_sample_rate_t sample_rate) {
    using std::cos;
    using std::pow;
    using std::sin;
    using std::numbers::pi;

    for (auto &band : bands) {
        float f0 = band.frequency;
        if (f0 <= 0.0f)
            f0 = 20.0f;
        if (f0 > sample_rate * 0.5f)
            f0 = sample_rate * 0.5f - 1.0f;

        float A = pow(10.0f, band.gain / 40.0f);
        float Q = 1.5f;
        float w0 = 2.0f * pi * f0 / sample_rate;
        float cw = cos(w0);
        float sw = sin(w0);
        float alpha = sw / (2.0f * Q);

        // peaking
        float b0 = 1.0f + alpha * A;
        float b1 = -2.0f * cw;
        float b2 = 1.0f - alpha * A;
        float a0 = 1.0f + alpha / A;
        float a1 = -2.0f * cw;
        float a2 = 1.0f - alpha / A;

        band.a0 = b0 / a0;
        band.a1 = b1 / a0;
        band.a2 = b2 / a0;
        band.b1 = a1 / a0;
        band.b2 = a2 / a0;
    }

    const std::size_t total_samples = audio_buffer.get_length();
    const std::size_t frames = total_samples / channels;

    std::vector z1(bands.size(), std::vector(channels, 0.0f));
    std::vector z2(bands.size(), std::vector(channels, 0.0f));

    for (std::size_t frame = 0; frame < frames; ++frame) {
        for (std::size_t ch = 0; ch < channels; ++ch) {
            const std::size_t idx = frame + ch;
            float s = Adapters::fv_amplitude(audio_buffer.raw()[idx]);

            for (std::size_t bi = 0; bi < bands.size(); ++bi) {
                auto &band = bands[bi];

                float out = band.a0 * s + z1[bi][ch];
                float new_z1 = band.a1 * s + z2[bi][ch] - band.b1 * out;
                float new_z2 = band.a2 * s - band.b2 * out;

                z1[bi][ch] = new_z1;
                z2[bi][ch] = new_z2;

                s = out;
            }

            audio_buffer.raw()[idx] = Adapters::from_fv_amplitude(s);
        }
    }
}

} // namespace HyperstreamRemixer::Sound::Effects
