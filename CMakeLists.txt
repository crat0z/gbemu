set(CMAKE_WARN_DEPRECATED FALSE)
cmake_minimum_required (VERSION 3.16)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

project(chip8emu CXX)

if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(USE_CLANG TRUE)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(USE_GCC TRUE)
elseif(MSVC)
    set(USE_MSVC TRUE)
endif()

if($<CONFIG:Debug> OR CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG_BUILD TRUE)
else()
    set(RELEASE_BUILD TRUE)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "shared libs")

# statically build on windows
if(USE_MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(SDL_LIBC ON CACHE BOOL "use SDL libc")
    set(SDL_FORCE_STATIC_VCRT ON CACHE BOOL "force static vcrt")
endif()

if(DEBUG_BUILD)
    if(USE_MSVC) # allows for hot reload while debugging with MSVC
        set(CMAKE_CXX_FLAGS_DEBUG "/ZI")
        set(CMAKE_SHARED_LINKER_FLAGS "/SAFESEH:NO")
        set(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO")
    endif()
else()
    # try to enable IPO/LTO
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result)
    if(result)
        set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
        # fixes cmake unable to find ar/ranlib on linux
        if(USE_CLANG)
            set(CMAKE_C_COMPILER_AR llvm-ar)
            set(CMAKE_C_COMPILER_RANLIB llvm-ranlib)
        elseif(USE_GCC)
            set(CMAKE_C_COMPILER_AR gcc-ar)
            set(CMAKE_C_COMPILER_RANLIB gcc-ranlib)
        endif()
    endif()
endif()

set(MY_INCLUDES ${chip8emu_SOURCE_DIR}/inc)

# sdl2
set(SDL2_INCLUDE_DIR ${chip8emu_SOURCE_DIR}/deps/SDL/include)

add_subdirectory(deps/SDL)

# sdl image
set(BUILD_SHOWIMAGE OFF CACHE BOOL "SDL_image showimage")
set(SUPPORT_JPG OFF CACHE BOOL "SDL_image JPG")
set(SUPPORT_PNG OFF CACHE BOOL "SDL_image PNG")
set(SUPPORT_WEBP OFF CACHE BOOL "SDL_image WEBP")

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

target_link_libraries(chip8emu PRIVATE SDL2-static SDL2::SDL2main SDL2_image imgui fmt::fmt Threads::Threads)

target_compile_features(chip8emu PRIVATE cxx_std_20)

# clang/gcc options
if(USE_CLANG OR USE_GCC)
    list(APPEND FLAGS "-Wall" "-Wextra" "-Wpedantic")
    if(RELEASE_BUILD)
        list(APPEND FLAGS "-Werror")
    endif()
else()
# MSVC/clang-cl options
    list(APPEND FLAGS "/permissive-" "/W4")
    if(RELEASE_BUILD)
        list(APPEND FLAGS "/WX")
    endif()
endif()

target_compile_options(chip8emu PRIVATE ${FLAGS})

