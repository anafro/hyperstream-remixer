#include "studio.h++"
#include "HyperstreamRemixer/Memory/unit.h++"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <GLFW/glfw3.h>

namespace HyperstreamRemixer::Studio {
using namespace Memory;

Studio::Studio() {
    this->reverb = new Reverb();
    this->speed = new Speed();
    this->remainder = new Remainder();
    this->lowpass = new Lowpass();
    this->noise = new Noise();
    this->crackle = new Crackle();
    this->audio = Audio::from_mp3_file({reverb, speed, remainder, lowpass, noise, crackle}, sound_path);
    LOG(audio.to_string());
}

void Studio::start() {
    glfwSetErrorCallback([](int code, const char *description) noexcept -> void {
        std::println(stderr, "GLFW Error {:x}: {}", code, description);
    });

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

        ImGui::Begin("Studio");
        ImGui::DragScalar("Reverb", ImGuiDataType_Double, &reverb->reverb, slider_step, &fx_reverb_min, &fx_reverb_max, slider_value_format);
        ImGui::DragScalar("Speed", ImGuiDataType_Double, &speed->speed, slider_step, &fx_speed_min, &fx_speed_max, slider_value_format);
        ImGui::DragScalar("Lowpass Cutoff", ImGuiDataType_Double, &lowpass->cutoff, slider_step, &fx_lowpass_cutoff_min, &fx_lowpass_cutoff_max, slider_value_format);
        ImGui::DragScalar("Noise", ImGuiDataType_Double, &noise->strength, slider_step, &fx_speed_min, &fx_speed_max, slider_value_format);
        ImGui::DragScalar("Crackle", ImGuiDataType_Double, &crackle->probability, slider_step, &fx_speed_min, &fx_speed_max, slider_value_format);

        if (ImGui::Button("Quit")) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        ImGui::End();

        ImGui::Render();
        int display_w;
        int display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
} // namespace HyperstreamRemixer::Studio
