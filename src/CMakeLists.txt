set(CMAKE_CXX_STANDARD 20)

add_executable(chip8emu main.cpp global.cpp)

add_subdirectory(gui)
add_subdirectory(core)
add_subdirectory(input)
