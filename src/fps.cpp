#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stdint.h>
#include <math.h>
#include "fps.h"

// Internal calculation of Frames per Second
// Basic implementation, maybe there is a better implementation but nahh
static float calculateFPS_Internal()
{
    static uint32_t lastTime = 0;
    static uint32_t frameCount = 0;
    static float fps = 0.0f;

    uint32_t currentTime = SDL_GetTicks();
    frameCount++;

    if (currentTime - lastTime >= 1000)
    {
        fps = frameCount * 1000.0f / (currentTime - lastTime);
        lastTime = currentTime;
        frameCount = 0;
    }

    return fps;
}

// Wrapper for FPS. Probably there will be more here later...
float calculateFPS_Float()
{
    return calculateFPS_Internal();
}