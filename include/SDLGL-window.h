#ifndef SDLGL_WINDOW_H
#define SDLGL_WINDOW_H

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <string>

class SDLGLwindow
{
private:
    std::string title;
    GLuint width, height;
    SDL_Window *window;
    SDL_GLContext glContext;
    int vsync;

public:
    SDLGLwindow(const std::string &title, GLuint width, GLuint height, int vsync);
    ~SDLGLwindow();

    bool init();
    SDL_Window *getWindow() const;
    SDL_GLContext getGLContext() const;
    void activateVsync(int vsync);
};

#endif