#ifndef RESOURCES_H
#define RESOURCES_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *loadImage(const char *filePath, int *width, int *height);

#endif
