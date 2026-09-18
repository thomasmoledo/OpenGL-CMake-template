cmake_minimum_required(VERSION 3.20)
project(MyProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL3IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3MIXER_VENDORED ON CACHE BOOL "" FORCE)
set(SDLIMAGE_TESTS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_TESTS OFF CACHE BOOL "" FORCE)

# Disable AVIF (avoids needing NASM for dav1d)
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)

include(FetchContent)

# --- SDL3 ---
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.16   # pin to a specific stable tag
    GIT_SHALLOW TRUE
)

# --- SDL3_image ---
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.4.6
    GIT_SHALLOW TRUE
)

# --- SDL3_mixer ---
FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-3.2.4
    GIT_SHALLOW TRUE
)

# Options for the SDL satellite libs (set before MakeAvailable)
set(SDL3IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3MIXER_VENDORED ON CACHE BOOL "" FORCE)
set(SDLIMAGE_TESTS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3 SDL3_image SDL3_mixer)

# --- GLAD2 (locally generated, not fetched) ---
add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

# --- Your executable ---
add_executable(MyProject src/main.cpp)

target_link_libraries(MyProject PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    glad
    opengl32
)

target_include_directories(MyProject PRIVATE external/glad/include)

# Copy SDL3 DLLs next to the exe after build (Windows)
if (WIN32)
    add_custom_command(TARGET MyProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:MyProject>
            $<TARGET_FILE_DIR:MyProject>
        COMMAND_EXPAND_LISTS
    )
endif()