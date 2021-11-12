set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")

# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(chip8emu main.cpp global.cpp)

add_subdirectory(gui)
add_subdirectory(core)
add_subdirectory(input)
