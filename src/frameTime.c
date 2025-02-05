#include "frameTime.h"

void initFrameTimer(FrameTimer *timer) {
  timer->deltaTime = 0.0f;
  timer->lastFrame = SDL_GetTicks() / 1000.0f; // Zeit in Sekunden
  timer->fps = 0.0f;
  timer->stableFPS = 0.0f;
  timer->timeAccumulator = 0.0f;
  timer->frameCount = 0;
}

void updateFrameTimer(FrameTimer *timer) {

  // calculating delta time and the Frame Rate per second. the "StableFPS" is
  // putting out the fps value every new second. so it's better to see the
  // average value.
  float currentFrame = SDL_GetTicks() / 1000.0f;
  timer->deltaTime = currentFrame - timer->lastFrame;
  timer->lastFrame = currentFrame;
  timer->fps = 1.0f / timer->deltaTime; // FPS for current frame

  // accumulation of time
  timer->timeAccumulator += timer->deltaTime;
  timer->frameCount++;

  // stableFPS every second
  if (timer->timeAccumulator >= 1.0f) {
    timer->stableFPS = timer->frameCount /
                       timer->timeAccumulator; // average Framerate every second
    timer->frameCount = 0;
    timer->timeAccumulator = 0.0f;
  }
}
