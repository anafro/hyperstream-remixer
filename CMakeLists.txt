cmake_minimum_required(VERSION 3.10)

project(HyperstreamRemixer)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_path(MINIMP3_INCLUDE_DIRS "minimp3/minimp3.h")
file(GLOB_RECURSE SOURCES "src/HyperstreamRemixer/*.c++" "src/Freeverb/Components/*.cpp")
add_executable(HyperstreamRemixer ${SOURCES})



target_include_directories(HyperstreamRemixer PRIVATE src)
target_link_libraries(HyperstreamRemixer PRIVATE SDL2::SDL2)
target_link_libraries(HyperstreamRemixer PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
target_include_directories(HyperstreamRemixer PRIVATE ${MINIMP3_INCLUDE_DIRS})

add_custom_command(
        TARGET HyperstreamRemixer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:HyperstreamRemixer>/assets"
)
