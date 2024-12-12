#include <stdio.h>
#include <SDL.h>

// Test for commit :D

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL konnte nicht initialisiert werden: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("LunaCore",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("Fenster konnte nicht erstellt werden: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event e;
    int quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        SDL_Delay(16); // Begrenze die CPU-Auslastung etwas
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
