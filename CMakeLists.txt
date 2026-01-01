cmake_minimum_required(VERSION 3.10)

project(HyperstreamRemixer)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_path(MINIMP3_INCLUDE_DIRS "minimp3/minimp3.h")
file(GLOB_RECURSE SOURCES "src/HyperstreamRemixer/*.c++" "src/Freeverb/Components/*.cpp")

set(IMGUI_BACKEND src/ImGui/backends/imgui_impl_glfw.cpp src/ImGui/backends/imgui_impl_opengl3.cpp)

file(GLOB IMGUI_SOURCES
        src/ImGui/imgui.cpp
        src/ImGui/imgui_draw.cpp
        src/ImGui/imgui_tables.cpp
        src/ImGui/imgui_widgets.cpp
        src/ImGui/imgui_demo.cpp
        src/ImGui/backends/imgui_impl_sdl2.cpp
        ${IMGUI_BACKEND}
)

add_executable(HyperstreamRemixer ${SOURCES} ${IMGUI_SOURCES})

add_compile_definitions(SDL_MAIN_HANDLED)
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE "GLVND")
if(WIN32)
  set_target_properties(HyperstreamRemixer PROPERTIES WIN32_EXECUTABLE OFF)
  target_link_libraries(HyperstreamRemixer PRIVATE opengl32)
elseif(UNIX)
  find_package(OpenGL REQUIRED)
  target_link_libraries(HyperstreamRemixer PRIVATE OpenGL::GL)
endif()


target_include_directories(HyperstreamRemixer PRIVATE src)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(HyperstreamRemixer PRIVATE glfw)
target_include_directories(HyperstreamRemixer PRIVATE src/ImGui)
target_link_libraries(HyperstreamRemixer PRIVATE SDL2::SDL2)
target_link_libraries(HyperstreamRemixer PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
target_include_directories(HyperstreamRemixer PRIVATE ${MINIMP3_INCLUDE_DIRS})

add_custom_command(
        TARGET HyperstreamRemixer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/music"
        "$<TARGET_FILE_DIR:HyperstreamRemixer>/music"
)
