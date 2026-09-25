/**
 * @file main.cpp
 * @brief A simple program that creates a window with a green background.
 * @date 2026-09-25
 */

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>

/**
 Note here that `constexpr` is basically a compile-time version of `const`.  They do the same thing, for our purposes,
 tho.
 */
 // The size of our literal game window
constexpr int WINDOW_WIDTH = 640 * 2,
WINDOW_HEIGHT = 480 * 2;

// The background colours may change in the course of our games, so they can also be
// variables
constexpr float BG_RED = 0.1922f,
BG_BLUE = 0.549f,
BG_GREEN = 0.9059f,
BG_OPACITY = 1.0f;

// it's good practice to add a `g_` to global variables' names
SDL_Window* g_display_window = nullptr;
bool g_game_is_running = true;

int main(int argc, char* argv[])
{
    // Initialising
    SDL_Init(SDL_INIT_VIDEO);

    g_display_window = SDL_CreateWindow("Hello, World!",
        WINDOW_WIDTH,WINDOW_HEIGHT, SDL_WINDOW_OPENGL);


    // If the window could not be created, then we should quit the program
    if (g_display_window == nullptr)
    {
        std::cerr << "Error: SDL window could not be created.\n";
        SDL_Quit();
        return 1;
    }

    // Create an OpenGL context for an OpenGL window...
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);

    // ...and make it the context we are currently working in
    SDL_GL_MakeCurrent(g_display_window, context);
    gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);

    /**
     The `SDL_Event` type is a union that contains structures for the different event types, like quitting or closing.
     */
    SDL_Event event;
    while (g_game_is_running)
    {
        /**
         Basically, while OpenGL is polling, or expecting, an event from the user, don't do anything.
         */
        while (SDL_PollEvent(&event))
        {
            /**
             The only types of events we are expecting thus far are simple those that end our OpenGL program.
             This will change soon.
             */
            if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            {
                g_game_is_running = false;
            }
        }

        // Quite simply: clear the space in memory holding our colours
        glClear(GL_COLOR_BUFFER_BIT);

        // Update a window with whatever OpenGL is rendering
        SDL_GL_SwapWindow(g_display_window);
    }

    SDL_Quit();
    return 0;
}