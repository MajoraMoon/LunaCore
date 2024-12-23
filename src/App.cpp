#include <vector>

#include "SDLGLwindow.h"
#include "Renderer.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include <SDL2/SDL_image.h>

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

    renderer.setup();

    while (renderer.isRunning())
    {
        renderer.handleInputEvents();
        renderer.render();
    }

    return 0;
}
