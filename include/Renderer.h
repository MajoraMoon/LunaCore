#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "SDLGLwindow.h"
#include <stdexcept>
#include "fps.h"

class Renderer
{
private:
    SDL_Window *window;
    SDL_GLContext glContext;
    SDLGLwindow &sdgl_window;
    bool running;
    /* data */
public:
    Renderer(SDL_Window *window, SDL_GLContext glContext, SDLGLwindow &sdgl_window);
    void render();
    void handleInputEvents();
    bool isRunning() const;
};

#endif
