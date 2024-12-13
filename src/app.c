#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "fps.h"

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

    if (TTF_Init() == -1)
    {
        SDL_Log("Could not initiate SDL-ttf: %s\n", TTF_GetError());
        SDL_Quit();
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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initiate SDL-Window: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Renderer uses hardware acceleration and vsync.
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Could not create SDL-Renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("../assets/fonts/Roboto/Roboto-Regular.ttf", 200);

    if (font == NULL)
    {
        SDL_Log("Could not load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color textColor = {255, 255, 255, 255}; // white color
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Hello World !", textColor);

    if (textSurface == NULL)
    {
        SDL_Log("Could not change text from font: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (textTexture == NULL)
    {
        printf("Could not create texture: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Event event;

    // activates V-sync. Similar to the GLFW function "glfwSwapInterval(int a)"
    // SDL_GL_SetSwapInterval(1);

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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect textRect = {100, 100, 600, 100};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_RenderPresent(renderer);

        fps = calculateFPS_Float();
        SDL_Log("FPS: %.2f\n", fps);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_Log("Quitted successfully");

    return 0;
}
