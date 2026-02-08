cmake_minimum_required(VERSION 3.10)

project(HyperstreamRemixer)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -std=c++23 -stdlib=libstdc++ -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -stdlib=libstdc++  -fno-omit-frame-pointer")
set(FMT_USE_LOCALE OFF CACHE BOOL "Disable locale support")

file(GLOB_RECURSE SOURCES "src/HyperstreamRemixer/*.c++" "src/Freeverb/Components/*.cpp")

add_executable(HyperstreamRemixer ${SOURCES})

add_compile_definitions(SDL_MAIN_HANDLED)
cmake_policy(SET CMP0072 NEW)

find_package(SDL2 CONFIG REQUIRED)
find_path(MINIMP3_INCLUDE_DIRS "minimp3/minimp3.h")

find_package(fmt CONFIG REQUIRED)
target_link_libraries(HyperstreamRemixer PRIVATE fmt::fmt)

find_package(mp3lame CONFIG REQUIRED)
target_link_libraries(HyperstreamRemixer PRIVATE mp3lame::mp3lame)

find_package(argparse CONFIG REQUIRED)
target_link_libraries(HyperstreamRemixer PRIVATE argparse::argparse)

target_include_directories(HyperstreamRemixer PRIVATE src)
target_link_libraries(HyperstreamRemixer PRIVATE SDL2::SDL2)
target_include_directories(HyperstreamRemixer PRIVATE ${MINIMP3_INCLUDE_DIRS})

find_package(AWSSDK CONFIG COMPONENTS s3 REQUIRED)
target_include_directories(HyperstreamRemixer PRIVATE ${AWSSDK_INCLUDE_DIRS})
target_link_libraries(HyperstreamRemixer PRIVATE ${AWSSDK_LIBRARIES})

find_package(amqpcpp CONFIG REQUIRED)
target_link_libraries(HyperstreamRemixer PRIVATE amqpcpp)

target_link_libraries(HyperstreamRemixer PRIVATE stdc++)
