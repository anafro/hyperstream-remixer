#pragma once
#include "HyperstreamRemixer/Sound/Effects/crackle.h++"
#include "HyperstreamRemixer/Sound/Effects/lowpass.h++"
#include "HyperstreamRemixer/Sound/Effects/noise.h++"
#include "HyperstreamRemixer/Sound/Effects/remainder.h++"
#include "HyperstreamRemixer/Sound/Effects/reverb.h++"
#include "HyperstreamRemixer/Sound/Effects/speed.h++"
#include <HyperstreamRemixer/Sound/Buffering/audio.h++>

namespace HyperstreamRemixer::Studio {
using namespace Sound::Buffering;

constexpr const char *sound_path = "./music/Notification.mp3";
constexpr const char *window_title = "Hyperstream Remixer: Studio";
constexpr std::size_t window_width = 800;
constexpr std::size_t window_height = 280;
constexpr double slider_step = 5_percent;
constexpr const char *slider_value_format = "%.2f";
class Studio final {
    Unit<Audio> audio;
    Reverb *reverb;
    Speed *speed;
    Remainder *remainder;
    Lowpass *lowpass;
    Noise *noise;
    Crackle *crackle;

  public:
    Studio();
    void start();
};
} // namespace HyperstreamRemixer::Studio
