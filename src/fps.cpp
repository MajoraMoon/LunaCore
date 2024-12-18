#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stdint.h>

#include "fps.h"

// Internal calculation of Frames per Second
static int calculateFPS_Internal()
{
    static uint32_t lastTime = 0;
    static int frames = 0;
    int fps = -1;

    uint32_t currentTime = SDL_GetTicks();
    frames++;

    if (currentTime >= lastTime + 1000)
    {
        fps = frames;
        frames = 0;
        lastTime = currentTime;
    }

    return fps;
}

void OutPut_fps()
{
    int fps = calculateFPS_Internal();

    if (fps >= 0)
    {
        SDL_Log("FPS: %d\n", fps);
    }
}
