#include "SDLGLwindow.h"
#include "Renderer.h"

int main(int argc, char *argv[])
{

    SDLGLwindow window("LunaCore", 1080, 720);

    if (!window.init())
    {
        SDL_Log("Failed to initialize the SDLGLwindow\n");

        return 1;
    }
    // Renderer gets the SDL window instance, the gl context and the SDLGLwindow object itself
    Renderer renderer(window.getSDLWindow(), window.getGLContext(), window);

    renderer.setup();

    while (renderer.isRunning())
    {
        renderer.handleInputEvents();
        renderer.render();
    }

    return 0;
}
