#ifndef SDLGL_WINDOW_H
#define SDLGL_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

public:
    SDLGLwindow(const std::string &title, GLuint width, GLuint height);
    ~SDLGLwindow();

    bool init();
    SDL_Window *getWindow() const;
    SDL_GLContext getGLContext() const;
    void activateVsync(int vsync);
    GLuint getInitWidth() const;
    GLuint getInitHeight() const;
};

#endif