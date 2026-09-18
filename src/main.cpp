#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cstdio>
#include <iostream>

constexpr int WINDOW_WIDTH = 640,
WINDOW_HEIGHT = 480;

constexpr float BG_RED = 0.1922f,
BG_BLUE = 0.549f,
BG_GREEN = 0.9059f,
BG_OPACITY = 1.0f;

SDL_Window* g_display_window = nullptr;
bool g_game_is_running = true;

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Error: SDL could not initialize. " << SDL_GetError() << std::endl;
        return 1;
    }

    // Request an OpenGL context (GLAD needs a valid profile/version set beforehand)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // SDL3's SDL_CreateWindow no longer takes x/y position args
    g_display_window = SDL_CreateWindow("Hello, World!",
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (g_display_window == nullptr)
    {
        std::cerr << "Error: SDL window could not be created. " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Optional: center the window (SDL_WINDOWPOS_CENTERED is now used this way)
    SDL_SetWindowPosition(g_display_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);

    if (context == nullptr)
    {
        std::cerr << "Error: OpenGL context could not be created. " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(g_display_window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_MakeCurrent(g_display_window, context);

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize GLAD");
        return 1;
    }

    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);

    SDL_Event event;
    while (g_game_is_running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT ||
                event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            {
                g_game_is_running = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(g_display_window);
    }

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(g_display_window);
    SDL_Quit();
    return 0;
}