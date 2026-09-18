cmake_minimum_required(VERSION 3.25)

project(OpenGLGame
    VERSION 1.0
    LANGUAGES C CXX
)

# --------------------------------------------------
# C++ standard
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------
# SDL3
# --------------------------------------------------

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL
    ${CMAKE_CURRENT_BINARY_DIR}/SDL
    EXCLUDE_FROM_ALL
)

# --------------------------------------------------
# GLAD2
# --------------------------------------------------

add_library(glad STATIC
    external/glad/src/gl.c
)

target_include_directories(glad
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

# --------------------------------------------------
# Game executable
# --------------------------------------------------

add_executable(${PROJECT_NAME}
    src/main.cpp
)

# --------------------------------------------------
# Libraries
# --------------------------------------------------

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3
        glad
)

# --------------------------------------------------
# OpenGL
# --------------------------------------------------

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            opengl32
    )
endif()