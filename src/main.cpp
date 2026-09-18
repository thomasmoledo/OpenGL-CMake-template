#include <iostream>

#include <SDL3/SDL.h>
#include <glad/gl.h>

int main()
{
    // --------------------------------------------
    // Initialize SDL
    // --------------------------------------------

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: "
                  << SDL_GetError()
                  << '\n';

        return 1;
    }

    // --------------------------------------------
    // Request OpenGL 4.6 Core
    // --------------------------------------------

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MAJOR_VERSION,
        4
    );

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MINOR_VERSION,
        6
    );

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    SDL_GL_SetAttribute(
        SDL_GL_DOUBLEBUFFER,
        1
    );

    // --------------------------------------------
    // Create window
    // --------------------------------------------

    SDL_Window* window = SDL_CreateWindow(
        "OpenGL + SDL3 + GLAD2",
        1280,
        720,
        SDL_WINDOW_OPENGL
    );

    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: "
                  << SDL_GetError()
                  << '\n';

        SDL_Quit();
        return 1;
    }

    // --------------------------------------------
    // Create OpenGL context
    // --------------------------------------------

    SDL_GLContext context =
        SDL_GL_CreateContext(window);

    if (!context)
    {
        std::cerr << "SDL_GL_CreateContext failed: "
                  << SDL_GetError()
                  << '\n';

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    // --------------------------------------------
    // Load OpenGL functions with GLAD2
    // --------------------------------------------

    int version = gladLoadGL(
        reinterpret_cast<GLADloadfunc>(
            SDL_GL_GetProcAddress
        )
    );

    if (version == 0)
    {
        std::cerr << "Failed to initialize GLAD2\n";

        SDL_GL_DestroyContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    std::cout
        << "Loaded OpenGL "
        << GLAD_VERSION_MAJOR(version)
        << "."
        << GLAD_VERSION_MINOR(version)
        << '\n';

    // --------------------------------------------
    // Main loop
    // --------------------------------------------

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // Clear screen
        glClearColor(
            0.1f,
            0.1f,
            0.15f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // --------------------------------------------
    // Cleanup
    // --------------------------------------------

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}