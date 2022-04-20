#define GLEW_STATIC

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <format>

#include "logger.h"
#include "ttfloader.h"

// rgba
const float CLEAR_COLOR[4] = {.0, .0, .0, .0};

void update(SDL_Window *window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
    SDL_GL_SwapWindow(window);
}

int main(int num_arguments, char **arguments) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //OpenGL core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); //OpenGL 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); //OpenGL 4
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }

    SDL_DisplayMode displayMode;

    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        logger::warn(std::string("SDL_GetCurrentDisplayMode failed: ") + SDL_GetError());
    }

    logger::info(std::string("Found display:"));
    logger::info(std::string("  -> width: " + std::to_string(displayMode.w)));
    logger::info(std::string("  -> height: " + std::to_string(displayMode.h)));

    SDL_Window *window = SDL_CreateWindow("GLFontRenderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          displayMode.w / 2, displayMode.h / 2, SDL_WINDOW_OPENGL);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    if (gl_context == nullptr) {
        logger::info(std::string("couldn't create gl-context: ") + SDL_GetError());
        logger::info(std::string("  -> gl_minor: ") + std::to_string(SDL_GL_CONTEXT_MINOR_VERSION));
        logger::info(std::string("  -> gl_major: ") + std::to_string(SDL_GL_CONTEXT_MAJOR_VERSION));
        return 1;
    }

    logger::info("Created window:");
    logger::info(std::string("  -> width: ") + std::to_string(displayMode.w / 2));
    logger::info(std::string("  -> height: ") + std::to_string(displayMode.h / 2));

    if (!window) {
        logger::info(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
        return 1;
    }

    GLenum result = glewInit();
    if (result != GLEW_OK) {
        logger::info(std::string("glewInit failed: ") + (const char *) glewGetErrorString(result));
        logger::info(std::string("result: ") + std::to_string(result));
        return 1;
    }

    const FT_Face fontFace = ttfloader::load_ttf_from_file("calibri.ttf", 32);

    bool running = true;
    SDL_Event ev;

    while (running) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
        }
        update(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
