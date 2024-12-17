#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#include <fps.h>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
bool quit = false;
float fps = 0;

int main(int argc, char *argv[])
{
    
    // First, initiate SDL. Here initiating sdl-subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initiate SDL: %s\n", SDL_GetError);
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create Window instance
    SDL_Window *window = SDL_CreateWindow("LunaCore",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCR_WIDTH,
                                          SCR_HEIGHT,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initiate SDL-OpenGL-Window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;

    // activates V-sync. Similar to the GLFW function "glfwSwapInterval(int a)". So it is the same lol
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

            // Detects if the key below is pressed down
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = true;
                }
            }
        }

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);

        fps = calculateFPS_Float();
        SDL_Log("FPS: %.2f\n", fps);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_Log("Quitted successfully");

    return 0;
}


