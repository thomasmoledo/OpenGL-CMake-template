#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cstdio>
#include <cmath>
#include <iostream>

constexpr int WINDOW_WIDTH = 640,
WINDOW_HEIGHT = 480;

constexpr float BG_RED = 0.1922f,
BG_BLUE = 0.549f,
BG_GREEN = 0.9059f,
BG_OPACITY = 1.0f;

SDL_Window* g_display_window = nullptr;
bool g_game_is_running = true;

// --- Shader sources ---
static const char* VERTEX_SHADER_SRC = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform float uScale;
void main()
{
    gl_Position = vec4(aPos.x * uScale, aPos.y * uScale, aPos.z, 1.0);
}
)";

static const char* FRAGMENT_SHADER_SRC = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // red
}
)";

GLuint g_shader_program = 0;
GLuint g_vao = 0, g_vbo = 0;
GLint g_scale_location = -1;

GLuint compile_shader(GLenum type, const char* src)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "Shader compile error: " << info_log << std::endl;
    }
    return shader;
}

void initialize_triangle()
{
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, VERTEX_SHADER_SRC);
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);

    g_shader_program = glCreateProgram();
    glAttachShader(g_shader_program, vertex_shader);
    glAttachShader(g_shader_program, fragment_shader);
    glLinkProgram(g_shader_program);

    GLint success;
    glGetProgramiv(g_shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetProgramInfoLog(g_shader_program, 512, nullptr, info_log);
        std::cerr << "Shader link error: " << info_log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    g_scale_location = glGetUniformLocation(g_shader_program, "uScale");

    float vertices[] = {
        0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    glGenVertexArrays(1, &g_vao);
    glGenBuffers(1, &g_vbo);

    glBindVertexArray(g_vao);

    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void render_triangle()
{
    // Pulse scale between ~0.5 and ~1.0 using a sine wave over time
    float time_seconds = SDL_GetTicks() / 1000.0f;
    float pulse = 0.75f + 0.25f * std::sin(time_seconds * 3.0f);

    glUseProgram(g_shader_program);
    glUniform1f(g_scale_location, pulse);

    glBindVertexArray(g_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

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

    initialize_triangle();

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
        render_triangle();
        SDL_GL_SwapWindow(g_display_window);
    }

    glDeleteVertexArrays(1, &g_vao);
    glDeleteBuffers(1, &g_vbo);
    glDeleteProgram(g_shader_program);

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(g_display_window);
    SDL_Quit();
    return 0;
}