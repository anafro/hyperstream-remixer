#include <chrono>
#include <iostream>
#include <vector>
#include <memory>
#include <HyperstreamRemixer/Audio/audio-system.h++>
#include <HyperstreamRemixer/Audio/Buffering/audio.h++>
#include <HyperstreamRemixer/Audio/Effects/reverb.h++>


using HyperstreamRemixer::Audio::initialize_remixer;
using HyperstreamRemixer::Audio::clean_up_remixer;
using HyperstreamRemixer::Audio::Buffering::Audio;
using HyperstreamRemixer::Exceptions::HyperstreamException;
using namespace HyperstreamRemixer::Audio::Effects;

int main(int argc, char* argv[]) {
    try {
        initialize_remixer();
        const std::vector<AudioEffect*> effects = {
            new Reverb()
        };
        const std::unique_ptr<Audio> audio(Audio::from_mp3_file(effects, "assets/Omnia, Ira - The Fusion.mp3"));
        audio->play();

        clean_up_remixer();

        return EXIT_SUCCESS;
    } catch (const HyperstreamException& exception) {
        std::cerr << "Hyperstream Remixer crashed:" << std::endl;
        std::cerr << "See exception to find cause:" << std::endl;
        std::cerr << exception.what() << std::endl;

        return EXIT_FAILURE;
    }
}
