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

    // Überprüft, ob mindestens 1 Sekunde vergangen ist, um FPS zu berechnen
    if (currentTime >= lastTime + 1000)
    {
        fps = frames;           // FPS berechnen (Frames pro Sekunde)
        frames = 0;             // Zähler zurücksetzen
        lastTime = currentTime; // Letzte Aktualisierung der Zeit
    }

    return fps;
}

// Wrapper für FPS
void OutPut_fps()
{
    int fps = calculateFPS_Internal(); // Aktuellen FPS berechnen

    // Überprüft, ob FPS einen gültigen Wert erreicht hat und gibt ihn aus
    if (fps >= 0)
    {
        SDL_Log("FPS: %d\n", fps);
    }
}
