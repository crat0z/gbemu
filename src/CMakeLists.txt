set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -march=native -pthread -lSDL2")

include_directories(${chip8emu_SOURCE_DIR}/inc)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(chip8emu main.cpp chip8.cpp graphics.cpp ctx.cpp)

