#include <HyperstreamRemixer/Debug/visual-audio-debugger.h++>
#include <HyperstreamRemixer/Sound/Buffering/audio.h++>
#include <HyperstreamRemixer/Sound/Effects/eq.h++>
#include <HyperstreamRemixer/Sound/Mp3/audio-system.h++>
#include <HyperstreamRemixer/Studio/studio.h++>
#include <iostream>

using namespace std::chrono_literals;
using namespace HyperstreamRemixer::Sound::Buffering;
using namespace HyperstreamRemixer::Exceptions;
using namespace HyperstreamRemixer::Sound;
using namespace HyperstreamRemixer::Sound::Effects;
using namespace HyperstreamRemixer::Studio;

auto main(int argc, char *argv[]) -> int {
    try {
        initialize_remixer();
        {
            // Studio studio;
            // studio.start();
            Audio::from_mp3_file({new Remainder(),
                                  new Speed(1.2),
                                  new Lowpass(0.3),
                                  new Reverb(0.2),
                                  new Noise(0.3),
                                  new Crackle(0.0003)},
                                 std::format("./music/{}.mp3", argv[1]))
                ->play();
        }
        clean_up_remixer();

        return EXIT_SUCCESS;
    } catch (const HyperstreamException &exception) {
        std::cerr << "Hyperstream Remixer crashed:" << '\n';
        std::cerr << "See exception to find cause:" << '\n';
        std::cerr << exception.what() << '\n';
    }
}
