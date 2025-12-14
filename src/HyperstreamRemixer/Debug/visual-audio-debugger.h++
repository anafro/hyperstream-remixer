#pragma once
#include "visual-audio-debugger-definitions.h++"
#include <HyperstreamRemixer/Environment/configuration.h++>
#include <HyperstreamRemixer/Sound/Buffering/audio.h++>
#include <HyperstreamRemixer/Sound/Effects/eq.h++>
#include <HyperstreamRemixer/Sound/Effects/remainder.h++>
#include <HyperstreamRemixer/Sound/Effects/reverb.h++>
#include <HyperstreamRemixer/Sound/Effects/speed.h++>

#ifdef REMIXER_VISUAL_DEBUGGER_ON
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <thread>
#endif

namespace HyperstreamRemixer::Debug {
using namespace Memory;
using namespace Sound::Buffering;
using namespace std::chrono_literals;

REMIXER_VISUAL_DEBUGGER_FUNCTION_DEFINITION() {
    static bool playing = false;
    static auto remainder = create<Remainder>(4.0s);
    static auto reverb = create<Reverb>(1.);
    static auto speed = create<Speed>();
    static auto eq = create<EQ>();
    static auto audio = object(Audio::from_mp3_file({*remainder, *reverb, *speed, *eq}, "music/Bruh.mp3", APPLY_FX_ON_PLAY));

    if (!playing) {
        playing = true;

        std::thread([]() -> void {
            while (playing) {
                audio->play();
            }
        }).detach();
    }

    ImGui::Begin("Hyperstream Remixer - Visual Debugger");
    ImGui::DragScalar("Reverb", ImGuiDataType_Double, &reverb->reverb, .05, &fx_reverb_min, &fx_reverb_max, "%.2f");
    ImGui::DragScalar("Speed", ImGuiDataType_Double, &speed->speed, .05, &fx_speed_min, &fx_speed_max, "%.2f");
    ImGui::SeparatorText("Multiband EQ");
    for (std::size_t i = 0; i < eq_bands; i++) {
        if (i != 0) {
            ImGui::SameLine();
        }

        ImGui::VSliderScalar(std::to_string(i).c_str(), ImVec2(18, 160), ImGuiDataType_Double, &eq->bands[i], &eq_gain_min, &eq_gain_max, "");
    }

    ImGui::End();
}

static void glfw_error_callback(const int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

inline void $show_visual_debugger() {
#ifdef REMIXER_VISUAL_DEBUGGER_ON
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    const auto glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "ImGui Demo", nullptr, nullptr);
    if (window == nullptr)
        exit(EXIT_FAILURE);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        $describe_visual_debugger_imgui_frame();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
