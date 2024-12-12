#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
bool quit = false;

int main(int argc, char const *argv[])
{
    // First, initiate SDL. Here initiating sdl-subsystem
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Could not initiate SDL: %s\n", SDL_GetError);
        return 1;
    }

    // Create Window instance
    SDL_Window *window = SDL_CreateWindow("LunaCore",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCR_WIDTH,
                                          SCR_HEIGHT,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        printf("Could not initiate SDL-Window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event event;

    // activates V-sync. Similar to the GLFW function "glfwSwapInterval(int a)"
    SDL_GL_SetSwapInterval(1);

    // main loop. Different than in OpenGL itself, the window does not needs to be updated every frame
    // The Main window is created until SDL_QUIT is set to true...
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            // exit the main loop and closes the window
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            // checks for the event type and what event option is called by the specific type.
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                printf("Resized Window: %d x %d\n", event.window.data1, event.window.data2);
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
