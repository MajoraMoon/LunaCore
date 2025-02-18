#ifndef RENDERER_H
#define RENDERER_H

// clang-format off

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "shader.h"
#include "resources.h"
#include "imgui_c.h"
#include "frameTime.h"

// clang-format on

typedef struct Renderer {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  GLuint texture;
  Shader shader;
} Renderer;

// setting up data before the actual render loop
void initRenderer(Renderer *renderer);

// use in render loop, OpenGL actual rendering
void renderFrame(Renderer *renderer, unsigned int srcWidth,
                 unsigned int srcHeight);

// Destroy data from OpenGL
void cleanupRenderer(Renderer *renderer);

#endif