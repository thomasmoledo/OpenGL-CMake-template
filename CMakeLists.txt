cmake_minimum_required(VERSION 3.20)
project(main LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# --- GLM ---
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	8d1fd52e5ab5590e2c81768ace50c72bae28f2ed #refs/tags/1.0.3
    GIT_SHALLOW TRUE
)

# Options for the SDL satellite libs (set before MakeAvailable)
set(SDL3IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3MIXER_VENDORED ON CACHE BOOL "" FORCE)
set(SDLIMAGE_TESTS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_TESTS OFF CACHE BOOL "" FORCE)

# Stop GLM from building a binary library
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)

# AVIF support pulls in dav1d, which needs a NASM assembler on Windows.
# Disabled by default so the project builds with no extra tools installed.
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3 SDL3_image SDL3_mixer glm)

# --- GLAD2 (locally generated, not fetched) ---
add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

# --- OpenGL ---
# find_package(OpenGL) resolves to the correct system GL library on
# Windows (opengl32), Linux (libGL), and macOS (OpenGL.framework),
# so linking OpenGL::GL works everywhere without platform-specific names.
find_package(OpenGL REQUIRED)

# --- Your executable ---
add_executable(main src/main.cpp)

target_link_libraries(main PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    glad
    OpenGL::GL
    glm::glm
)

target_include_directories(main PRIVATE external/glad/include)

# macOS caps out at OpenGL 4.1 core and Apple has deprecated the API
# (it still works, just no driver updates past 4.1). Silence the
# deprecation warnings that OpenGL headers emit on macOS.
if (APPLE)
    target_compile_definitions(main PRIVATE GL_SILENCE_DEPRECATION)
endif()

# Copy SDL3 DLLs next to the exe after build (Windows only —
# macOS/Linux resolve shared libraries via rpath automatically).
if (WIN32)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:main>
            $<TARGET_FILE_DIR:main>
        COMMAND_EXPAND_LISTS
    )
endif()