#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include <stdexcept>

#include <SDLGL.h>
#include <ImGuiSDLGL.h>

#include "Buffer.h"
#include "Shader.h"
#include "SDLGLwindow.h"

class Renderer
{
public:
    Renderer(SDL_Window *window, SDL_GLContext glContext, SDLGLwindow &sdgl_window);
    void setup();
    void render();
    void handleInputEvents();
    bool isRunning() const;
    void showInformationImGui();

private:
    enum VsyncMode
    {
        VSync_Off = 0,
        VSync_On = 1
    };

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

    VsyncMode currentVsyncMode;
    VsyncMode lastVsyncMode = VSync_Off;

    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArrayObject> vao;
    std::unique_ptr<VertexBufferObject> vbo;
    std::unique_ptr<ElementBufferObject> ebo;

    void activateVsync(VsyncMode vsync);
};

#endif
