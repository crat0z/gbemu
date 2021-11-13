set(CMAKE_WARN_DEPRECATED FALSE)
cmake_minimum_required (VERSION 3.16)

set(CMAKE_C_COMPILER_AR gcc-ar)
set(CMAKE_C_COMPILER_RANLIB gcc-ranlib)

project(chip8emu CXX)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(MY_INCLUDES ${chip8emu_SOURCE_DIR}/inc)

# temporary stuff, until fedora gets 2.0.17
set(SDL_JACK OFF)
set(SDL_PIPEWIRE OFF)
set(SDL_ESD OFF)
set(SDL_RPI OFF)
set(SDL2_INCLUDE_DIR ${chip8emu_SOURCE_DIR}/deps/SDL/include)

add_subdirectory(deps/SDL)

# sdl image
set(SUPPORT_JPG OFF)
set(SUPPORT_PNG OFF)
set(SUPPORT_WEBP OFF)
set(BUILD_SHOWIMAGE OFF)

add_subdirectory(deps/SDL_image)

set(SDL_IMAGE_INCLUDE_DIR ${chip8emu_SOURCE_DIR}/deps/SDL_image)

target_compile_definitions(SDL2_image PRIVATE -DLOAD_SVG)

# imgui
set(IMGUI_PATH ${chip8emu_SOURCE_DIR}/deps/imgui)
set(IMGUI_BACKENDS ${IMGUI_PATH}/backends)

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_BACKENDS}/imgui_impl_sdl.cpp ${IMGUI_BACKENDS}/imgui_impl_sdlrenderer.cpp)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_PATH} PUBLIC ${IMGUI_BACKENDS} PUBLIC ${SDL2_INCLUDE_DIR})

# fmtlib
add_subdirectory(deps/fmt)

find_package(Threads REQUIRED)

# this project
add_subdirectory(src)

target_include_directories(chip8emu PUBLIC ${SDL2_INCLUDE_DIR} PUBLIC ${IMGUI_PATH} PUBLIC ${IMGUI_BACKENDS} PUBLIC ${SDL_IMAGE_INCLUDE_DIR} PUBLIC ${MY_INCLUDES})

target_link_libraries(chip8emu SDL2 SDL2_image imgui fmt::fmt Threads::Threads)