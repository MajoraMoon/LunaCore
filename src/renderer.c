#include "renderer.h"

static FrameTimer frameTimer;

static void showInformationImGUI(float fps, float deltaTime, float stableFPS);

void initRenderer(Renderer *renderer) {

  // VSync is on (at least on standard settings. This will deactivate it before
  // the rendering)
  SDL_GL_SetSwapInterval(0);

  // clang-format off
    GLfloat vertexData[] = 
    {
        // positions        //colors         // texure coords
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // vertex 0 - bottom left position and rgb color - red
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // vertex 1 - bottom right position and rgb color - green
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // vertex 2 - top left position and rgb color - blue
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f    // vertex 3 - top right position and rgb color - red
    };
  // clang-format on

  // going through all corners of the rectangles, starting counting from 0.
  GLuint elementBufferData[] = {0, 1, 2, 1, 3, 2};

  glGenVertexArrays(1, &renderer->vao);
  glBindVertexArray(renderer->vao);

  glGenBuffers(1, &renderer->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

  glGenBuffers(1, &renderer->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferData),
               elementBufferData, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8,
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8,
                        (GLvoid *)(sizeof(GLfloat) * 3));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8,
                        (GLvoid *)(sizeof(GLfloat) * 6));
  glEnableVertexAttribArray(2);

  // create Shaders
  renderer->shader = createShader("../shader/vertexShader.vert",
                                  "../shader/fragmentShader.frag");

  int width, height;

  // load image for texture
  unsigned char *imgData = loadImage(
      "../assets/textures/rocky_terrain_diff_4k.jpg", &width, &height);

  if (imgData) {
    // load and create a texture
    glGenTextures(1, &renderer->texture);
    // all upcoming GL_TEXTURE_2D operations now have effect on this texture
    // object
    glBindTexture(GL_TEXTURE_2D, renderer->texture);

    // set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, imgData);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture\n");
  }

  free(imgData);

  initFrameTimer(&frameTimer);
}

void renderFrame(Renderer *renderer, unsigned int srcWidth,
                 unsigned int srcHeight) {

  // Frames information
  updateFrameTimer(&frameTimer);
  // transformations

  mat4 trans;

  glm_mat4_identity(trans);
  glm_scale(trans, (vec3){0.5f, 0.5f, 0.5f});
  float angle = frameTimer.lastFrame;
  glm_rotate(trans, angle, (vec3){0.0f, 0.0f, 1.0f});

  mat4 proj;
  float aspect = (float)srcWidth / (float)srcHeight;
  glm_ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f, proj);

  // actual rendering

  useShader(&renderer->shader);
  unsigned int transformLoc =
      glGetUniformLocation(renderer->shader.ID, "transform");

  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const GLfloat *)&trans);

  glViewport(0, 0, srcWidth, srcHeight);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, renderer->texture);
  glBindVertexArray(renderer->vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  // Dear ImGui set up
  ImGui_ImplOpenGL3_NewFrame_C();
  ImGui_ImplSDL3_NewFrame_C();
  ImGui_NewFrame_C();

  showInformationImGUI(frameTimer.fps, frameTimer.deltaTime,
                       frameTimer.stableFPS);

  ImGui_Render_C();
  ImGui_ImplOpenGL3_RenderDrawData_C();
}

void cleanupRenderer(Renderer *renderer) {
  glDeleteVertexArrays(1, &renderer->vao);
  glDeleteBuffers(1, &renderer->vbo);
  glDeleteBuffers(1, &renderer->ebo);
  glDeleteTextures(1, &renderer->texture);
}

static void showInformationImGUI(float fps, float deltaTime, float stableFPS) {

  ImGui_Begin_C("Semi-useful Information");
  ImGui_Text_c("Delta Time: %.5f", deltaTime);
  ImGui_Text_c("FPS (per second): %.1f", stableFPS);
  ImGui_VsyncCombo_C();
  ImGui_End_c();
}