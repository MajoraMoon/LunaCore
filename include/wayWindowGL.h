#ifndef WAYWINDOW_H

#define WAYWINDOW_H
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <glad/glad.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// initiate a SDL3 window
SDL_Window *initWayWindowGL(const char *title, const char *version,
                            unsigned int width, unsigned int height,
                            bool resizableWindow);

SDL_GLContext initOpenGLContext_and_glad(SDL_Window *window);

void cleanupWindow(SDL_Window *window, SDL_GLContext glContext);

#endif