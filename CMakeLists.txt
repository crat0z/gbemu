set(CMAKE_WARN_DEPRECATED FALSE)
cmake_minimum_required (VERSION 3.16)

project(chip8emu)

set(MY_INCLUDES ${chip8emu_SOURCE_DIR}/inc)

# temporary stuff, until fedora gets 2.0.17
set(SDL_JACK OFF)
set(SDL_PIPEWIRE OFF)
set(SDL_ESD OFF)
set(SDL_RPI OFF)
set(SDL2_INCLUDE_DIR ${chip8emu_SOURCE_DIR}/deps/SDL/include)

add_subdirectory(deps/SDL)

# imgui
set(IMGUI_PATH ${chip8emu_SOURCE_DIR}/deps/imgui)
set(IMGUI_BACKENDS ${IMGUI_PATH}/backends)

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_BACKENDS}/imgui_impl_sdl.cpp ${IMGUI_BACKENDS}/imgui_impl_sdlrenderer.cpp)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_PATH} PUBLIC ${IMGUI_BACKENDS} PUBLIC ${SDL2_INCLUDE_DIR})

add_subdirectory(src)

target_include_directories(chip8emu PUBLIC ${SDL2_INCLUDE_DIR} PUBLIC ${IMGUI_PATH} PUBLIC ${IMGUI_BACKENDS} PUBLIC ${MY_INCLUDES})

target_link_libraries(chip8emu SDL2 imgui)