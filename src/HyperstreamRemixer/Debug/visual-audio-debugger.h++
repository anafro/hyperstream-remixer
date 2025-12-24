#pragma once
#include "HyperstreamRemixer/Macros/coerce-inline.h++"
#include "HyperstreamRemixer/Sound/Effects/lowpass.h++"
#include <HyperstreamRemixer/Environment/configuration.h++>
#include <HyperstreamRemixer/Sound/Buffering/audio.h++>
#include <HyperstreamRemixer/Sound/Effects/eq.h++>
#include <HyperstreamRemixer/Sound/Effects/remainder.h++>
#include <HyperstreamRemixer/Sound/Effects/reverb.h++>
#include <HyperstreamRemixer/Sound/Effects/speed.h++>
#include <cstdlib>

#ifdef REMIXER_VISUAL_DEBUGGER_ON
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <print>
#include <thread>
#endif

namespace HyperstreamRemixer::Debug {
using namespace Memory;
using namespace Sound::Buffering;
using namespace std::chrono_literals;
inline constexpr const char *sound_file_path = "./music/Notification.mp3";
inline constexpr const char *window_title = "Effects";
inline constexpr const char *debugger_title = "Hyperstream Remixer - Visual Debugger";
inline constexpr const char *slider_value_format = "%.2f";
inline constexpr double slider_step = 0.050;
inline constexpr std::size_t eq_band_width = 18;
inline constexpr std::size_t eq_band_height = 160;
inline constexpr std::size_t window_width = 1080;
inline constexpr std::size_t window_height = 720;
inline constexpr double window_fill_r = 0.100;
inline constexpr double window_fill_g = 0.100;
inline constexpr double window_fill_b = 0.100;
inline constexpr double window_fill_a = 1.000;

__REMIXER_COERCE_INLINE void render_visual_audio_debugger(GLFWwindow *window, Allocation<Audio> &audio, Reverb *reverb, Speed *speed, Lowpass *lowpass) {
    static bool is_first_frame = true;

    if (is_first_frame) {
        is_first_frame = false;

        std::thread([&window, &audio]() -> void {
            while (glfwWindowShouldClose(window) == 0 and not audio.is_null()) {
                audio->play();
            }
        }).detach();
    }

    ImGui::Begin(debugger_title);
    ImGui::DragScalar("Reverb", ImGuiDataType_Double, &reverb->reverb, slider_step, &fx_reverb_min, &fx_reverb_max, slider_value_format);
    ImGui::DragScalar("Speed", ImGuiDataType_Double, &speed->speed, slider_step, &fx_speed_min, &fx_speed_max, slider_value_format);
    ImGui::DragScalar("Lowpass Cutoff", ImGuiDataType_Double, &lowpass->cutoff, slider_step, &fx_lowpass_cutoff_min, &fx_lowpass_cutoff_max, slider_value_format);

    if (ImGui::Button("Quit")) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    ImGui::End();
}

static void glfw_error_callback(const int error, const char *description) {
    std::println(stderr, "GLFW Error {}: {}", error, description);
}

inline void $show_visual_debugger() {
#ifdef REMIXER_VISUAL_DEBUGGER_ON
    auto *remainder = new Remainder(1s);
    auto *reverb = new Reverb();
    auto *speed = new Speed();
    auto *lowpass = new Lowpass();
    auto audio = object(Audio::from_mp3_file({remainder, reverb, speed, lowpass}, sound_file_path, APPLY_FX_ON_PLAY));

    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() == 0) {
        exit(EXIT_FAILURE);
    }

    const auto *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);
    if (window == nullptr) {
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    [[maybe_unused]] auto &imgui_io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        render_visual_audio_debugger(window, audio, reverb, speed, lowpass);

        ImGui::Render();
        int display_w;
        int display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(window_fill_r, window_fill_g, window_fill_b, window_fill_a);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
#endif
}
} // namespace HyperstreamRemixer::Debug
