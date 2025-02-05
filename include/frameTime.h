#ifndef FRAME_TIME_H
#define FRAME_TIME_H

#include <SDL3/SDL.h>

typedef struct FrameTimer {
  float deltaTime;
  float lastFrame;
  float fps;
  float stableFPS;
  float timeAccumulator;
  int frameCount;
} FrameTimer;

void initFrameTimer(FrameTimer *timer);

void updateFrameTimer(FrameTimer *timer);

#endif