#include "HyperstreamRemixer/Exceptions/hyperstream-exception.h++"
#include "HyperstreamRemixer/Sound/Effects/crackle.h++"
#include "HyperstreamRemixer/Sound/Effects/lowpass.h++"
#include "HyperstreamRemixer/Sound/Effects/noise.h++"
#include "HyperstreamRemixer/Sound/Effects/remainder.h++"
#include "HyperstreamRemixer/Sound/Effects/reverb.h++"
#include "HyperstreamRemixer/Sound/Effects/speed.h++"
#include <HyperstreamRemixer/Runtime/interrupt.h++>
#include <HyperstreamRemixer/Sound/Buffering/audio.h++>
#include <HyperstreamRemixer/Sound/Effects/eq.h++>
#include <HyperstreamRemixer/Sound/Mp3/audio-system.h++>
#include <argparse/argparse.hpp>
#include <fmt/format.h>
#include <iostream>

using namespace std::chrono_literals;
using namespace HyperstreamRemixer::Sound::Buffering;
using namespace HyperstreamRemixer::Exceptions;
using namespace HyperstreamRemixer::Sound;
using namespace HyperstreamRemixer::Sound::Effects;
using namespace HyperstreamRemixer::Runtime;

auto main(const int argc, char *argv[]) noexcept -> int {
    try {
        initialize_remixer();
        {
            argparse::ArgumentParser argument_parser{"HyperstreamRemixer"};

            argument_parser
                .add_argument("-i", "--input")
                .help("the filename to import original audio from");

            argument_parser
                .add_argument("-o", "--output")
                .help("the filename to export an audio file with effects");

            fx_speed_t speed;
            argument_parser
                .add_argument("-s", "--speed")
                .scan<'f', fx_speed_t>()
                .default_value(fx_speed_default)
                .help(fmt::format("speed between {}, and {}", fx_speed_min, fx_speed_max))
                .store_into(speed);

            fx_reverb_t reverb;
            argument_parser
                .add_argument("-r", "--reverb")
                .scan<'f', fx_reverb_t>()
                .default_value(fx_reverb_default)
                .help(fmt::format("reverb between {}, and {}", fx_reverb_min, fx_reverb_max))
                .store_into(reverb);

            fx_lowpass_parameter_t lowpass;
            argument_parser
                .add_argument("-l", "--lowpass")
                .scan<'f', fx_lowpass_parameter_t>()
                .default_value(fx_lowpass_cutoff_default)
                .help(fmt::format("lowpass between {}, and {}", fx_lowpass_cutoff_min, fx_lowpass_cutoff_max))
                .store_into(lowpass);

            fx_noise_strength_t noise;
            argument_parser
                .add_argument("-n", "--noise")
                .scan<'f', fx_noise_strength_t>()
                .default_value(fx_noise_strength_default)
                .help(fmt::format("noise between {}, and {}", fx_noise_strength_min, fx_noise_strength_max))
                .store_into(noise);

            fx_crackle_probability_t crackle;
            argument_parser
                .add_argument("-c", "--crackle")
                .scan<'f', fx_crackle_probability_t>()
                .default_value(fx_crackle_probability_default)
                .help(fmt::format("crackle between {}, and {}", fx_crackle_probability_min, fx_crackle_probability_max))
                .store_into(crackle);

            try {
                argument_parser.parse_args(argc, argv);
            } catch (const std::exception &argument_parsing_exception) {
                fmt::println(stderr, "{}\n", argument_parsing_exception.what());
                fmt::println(stderr, "{}\n", argument_parser.help().str());
                interrupt(CLI_BAD_SYNTAX);
            }

            if (argc == 1) {
                interrupt(MESSAGE_EXCHANGE_FEATURE_NOT_IMPLEMENTED);
            }

            if (not argument_parser.present("-i")) {
                interrupt(INPUT_AUDIO_FILEPATH_NOT_PROVIDED);
            }

            const std::string input_filepath = argument_parser.get("-i");
            std::vector<AudioEffect *> effects{
                new Remainder(3s),
                new Speed(speed),
                new Lowpass(lowpass),
                new Reverb(reverb),
                new Noise(noise),
                new Crackle(crackle)};

            auto audio = Audio::from_mp3_file(std::move(effects), input_filepath);

            if (auto output_filepath_opt = argument_parser.present<std::string>("-o")) {
                const std::string &output_filepath = *output_filepath_opt;
                audio->to_mp3_file(output_filepath);
            } else {
                audio->play();
            }
        }
        clean_up_remixer();

        return EXIT_SUCCESS;
    } catch (const HyperstreamException &exception) {
        std::cerr << "Error: " << exception.what() << '\n';
    }
}
