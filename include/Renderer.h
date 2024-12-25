#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad/glad.h>

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Shader.h"
#include "SDLGLwindow.h"
#include "fps.h"

class Renderer
{
private:
    SDL_Window *window;
    SDL_GLContext glContext;
    SDLGLwindow &sdgl_window;
    bool running;
    GLuint texture;

    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArrayObject> vao;
    std::unique_ptr<VertexBufferObject> vbo;
    std::unique_ptr<ElementBufferObject> ebo;

public:
    Renderer(SDL_Window *window, SDL_GLContext glContext, SDLGLwindow &sdgl_window);
    void setup();
    void render();
    void handleInputEvents();
    bool isRunning() const;
};

#endif
