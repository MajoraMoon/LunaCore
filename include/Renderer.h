#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include <stdexcept>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Shader.h"
#include "SDLGLwindow.h"

class Renderer
{
private:
    SDL_Window *window;
    SDL_GLContext glContext;
    SDLGLwindow &sdgl_window;
    bool running;
    GLuint texture;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float fps = 0.0f;
    float stableFPS = 0.0f;
    float timeAccumulator = 0.0f;
    int frameCount = 0;

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
    void showFPSWindow();
};

#endif
