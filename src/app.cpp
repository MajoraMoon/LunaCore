#include "SDLGL-window.h"
#include "Renderer.h"

int main(int argc, char *argv[])
{

    SDLGLwindow window("LunaCore", 1080, 720);

    if (!window.init())
    {
        SDL_Log("Failed to initialize the SDLGLwindow\n");

        return 1;
    }

    window.activateVsync(1);

    Renderer renderer(window.getWindow(), window.getGLContext(), window);

    while (renderer.isRunning())
    {
        renderer.handleInputEvents();
        renderer.render();
    }

    return 0;
}
