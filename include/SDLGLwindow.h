#ifndef SDLGL_WINDOW_H
#define SDLGL_WINDOW_H

#include <SDLGL.h>
#include <ImGuiSDLGL.h>

#include <iostream>
#include <string>

class SDLGLwindow
{
public:
    SDLGLwindow(const std::string &title, GLuint width, GLuint height);
    ~SDLGLwindow();

    bool init();
    SDL_Window *getSDLWindow() const;
    SDL_GLContext getGLContext() const;

    GLuint getInitWidth() const;
    GLuint getInitHeight() const;

private:
    std::string title;
    GLuint width, height;
    SDL_Window *window;
    SDL_GLContext glContext;
};

#endif