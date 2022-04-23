#define GLEW_STATIC

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "logger.h"
#include "ttfloader.h"

#include "render/gl_shader.h"
#include "render/gl_buffer.h"

// rgba
const float CLEAR_COLOR[4] = {.0, .0, .0, .0};

void update(SDL_Window *window, glfr::gl_shader& shader, glfr::gl_buffer& buffer) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);

    shader.bind();
    buffer.draw();
    shader.unbind();

    SDL_GL_SwapWindow(window);
}

int main(int num_arguments, char **arguments) {
    using namespace glfr;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }

    SDL_DisplayMode displayMode;

    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        logger::warn("SDL_GetCurrentDisplayMode: {}", SDL_GetError());
    }

    logger::info("Found display:");
    logger::info("  -> width: {}", displayMode.w);
    logger::info("  -> height: {}", displayMode.h);

    SDL_Window *window = SDL_CreateWindow("GLFontRenderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          displayMode.w / 2, displayMode.h / 2, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //OpenGL core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5); //OpenGL 5
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); //OpenGL 4

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    if (gl_context == nullptr) {
        logger::info("couldn't create gl-context: {}", SDL_GetError());
        logger::info("  -> gl_minor: {}", SDL_GL_CONTEXT_MINOR_VERSION);
        logger::info("  -> gl_major: {}", SDL_GL_CONTEXT_MAJOR_VERSION);
        return 1;
    }

    logger::info("Created window:");
    logger::info("  -> width: {}", displayMode.w / 2);
    logger::info("  -> height: {}", displayMode.h / 2);

    if (!window) {
        logger::info("SDL_CreateWindow failed: {}", SDL_GetError());
        return 1;
    }

    GLenum result = glewInit();
    if (result != GLEW_OK) {
        logger::info("glewInit failed: {}", (const char *) glewGetErrorString(result));
        logger::info("result: {}", result);
        return 1;
    }

    auto *font = ttfloader::load_font("calibri.ttf", 32);

    bool running = true;
    SDL_Event ev;

    const vertex vertices[] = {
            { -0.5, 0.5, 0.0, 0.0, 0.0, 0xFFFF0000 },
            { -0.5, -0.5, 0.0, 0.0, 1.0, 0xFF00FF00 },
            { 0.5, -0.5, 0.0, 1.0, 1.0, 0xFF0000FF },
            { 0.5, 0.5, 0.0, 1.0, 0.0, 0xFFFFFF00 }
    };

    uint32_t indices[] = {
            0, 1, 3, 3, 1, 2
    };

    gl_shader _test_shader;
    gl_buffer _test_buffer(vertices, 4, indices, 6);

    while (running) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
        }
        update(window, _test_shader, _test_buffer);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
