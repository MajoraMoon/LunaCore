#include <Renderer.h>

Renderer::Renderer(SDL_Window *window, SDL_GLContext glContext, SDLGLwindow &sdgl_window)
    : window(window), glContext(glContext), running(true), sdgl_window(sdgl_window) {}

void Renderer::render()
{
    glViewport(0, 0, sdgl_window.getInitWidth(), sdgl_window.getInitHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

    OutPut_fps();
}

void Renderer::handleInputEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        {
            running = false;
        }
    }
}

bool Renderer::isRunning() const
{
    return running;
}
