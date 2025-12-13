#include <HyperstreamRemixer/Debug/visual-audio-debugger.h++>
#include <HyperstreamRemixer/Sound/Buffering/audio.h++>
#include <HyperstreamRemixer/Sound/Effects/eq.h++>
#include <HyperstreamRemixer/Sound/Mp3/audio-system.h++>
#include <iostream>

using namespace std::chrono_literals;
using namespace HyperstreamRemixer::Sound::Buffering;
using namespace HyperstreamRemixer::Exceptions;
using namespace HyperstreamRemixer::Sound;
using namespace HyperstreamRemixer::Sound::Effects;
using namespace HyperstreamRemixer::Debug;

auto main(int argc, char *argv[]) -> int {
    try {
        initialize_remixer();
        $show_visual_debugger();
        clean_up_remixer();

        return EXIT_SUCCESS;
    } catch (const HyperstreamException &exception) {
        std::cerr << "Hyperstream Remixer crashed:" << '\n';
        std::cerr << "See exception to find cause:" << '\n';
        std::cerr << exception.what() << '\n';
    }
}
