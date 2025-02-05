#include <app.h>

typedef enum {
  VSyncOFF = 0,
  VSyncON = 1

} VsyncMode;

static VsyncMode currentVsyncMode = VSyncOFF;

// use this: updateVSync(currentVsyncMode == VSyncON ? VSyncOFF : VSyncON);
void updateVSync(VsyncMode newVSyncMode) {

  if (newVSyncMode != currentVsyncMode) {

    if (SDL_GL_SetSwapInterval((int)newVSyncMode) != true) {
      SDL_Log("Could not set Swap-Interval (VSync) correctly: %s\n",
              SDL_GetError());
    } else {
      currentVsyncMode = newVSyncMode;
    }
  }
}

static void showInformationImGUI(float fps, float deltaTime, float stableFPS) {

  ImGui_Begin_C("Semi-useful Information");
  ImGui_Text_c("FPS (per frame): %1f", fps);
  ImGui_Text_c("Delta Time: %.5f", deltaTime);
  ImGui_Text_c("FPS (per second): %.1f", stableFPS);
  ImGui_VsyncCombo_C();
  ImGui_End_c();
}

static unsigned char *loadImage(const char *filePath, int *width, int *height) {
  SDL_Surface *surface = IMG_Load(filePath);

  if (!surface) {
    fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
    return NULL;
  }

  *width = surface->w;
  *height = surface->h;

  // Berechne die Datenmenge – Achtung:
  // Häufig muss hier auch die Anzahl der Kanäle (Bytes per Pixel)
  // berücksichtigt werden!
  size_t dataSize =
      (*width) * (*height) * (SDL_BITSPERPIXEL(surface->format) / 8);
  unsigned char *byteDataImg = malloc(dataSize);
  if (!byteDataImg) {
    fprintf(stderr, "Memory allocation failed.\n");
    SDL_DestroySurface(surface);
    return NULL;
  }

  memcpy(byteDataImg, surface->pixels, dataSize);
  SDL_DestroySurface(surface);

  return byteDataImg;
}

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main(int argc, char *argv[]) {

  SDL_Window *window =
      initWayWindowGL("LunaCore", "0.1", SCR_WIDTH, SCR_HEIGHT, true);

  if (window == NULL) {
    SDL_Log("Something went wrong in setting up a SDL window.\n");
    return -1;
  }

  SDL_GLContext glContext = initOpenGLContext_and_glad(window);

  initImGUI(window, glContext);

  Shader shader = createShader("../shader/vertexShader.vert",
                               "../shader/fragmentShader.frag");

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

  GLuint vboID, vaoID, eboID;
  GLuint texture;
  GLsizei width, height;

  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);

  glGenBuffers(1, &vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

  glGenBuffers(1, &eboID);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8,
                        (GLvoid *)0);

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8,
                        (GLvoid *)(sizeof(GLfloat) * 3));

  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(elementBufferData) * sizeof(GLuint), elementBufferData,
               GL_STATIC_DRAW);

  // Pointing to the coords of the texture. With stride 8 and offset 6.
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8,
                        (GLvoid *)(sizeof(GLfloat) * 6));
  glEnableVertexAttribArray(2);

  // load and create a texture
  glGenTextures(1, &texture);
  // all upcoming GL_TEXTURE_2D operations now have effect on this texture
  // object
  glBindTexture(GL_TEXTURE_2D, texture);

  // set texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char *imgData = loadImage(
      "../assets/textures/rocky_terrain_diff_4k.jpg", &width, &height);

  if (imgData) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, imgData);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  else {
    printf("Failed to load texture\n");
  }

  free(imgData);

  bool running = true;

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  float fps = 0.0f;
  float stableFPS = 0.0f;
  float timeAccumulator = 0.0f;
  int frameCount = 0;

  // VSync is on (at least on standard settings. This will deactivate it before
  // the rendering)
  SDL_GL_SetSwapInterval(currentVsyncMode);

  // main render loop
  while (running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent_C(&event);

      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }

      // Key Press down
      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          running = false;
        }
      }
    }

    // calculating delta time and the Frame Rate per second. the "StableFPS" is
    // putting out the fps value every new second. so it's better to see the
    // average value.
    // Berechnung von deltaTime und FPS
    float currentFrame = SDL_GetTicks() / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    fps = 1.0f / deltaTime; // FPS for current frame

    // accumulation of time
    timeAccumulator += deltaTime;
    frameCount++;

    // stableFPS every second
    if (timeAccumulator >= 1.0f) {
      stableFPS =
          frameCount / timeAccumulator; // average Framerate every second
      frameCount = 0;
      timeAccumulator = 0.0f;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture);

    useShader(&shader);

    glBindVertexArray(vaoID);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Dear ImGui set up
    ImGui_ImplOpenGL3_NewFrame_C();
    ImGui_ImplSDL3_NewFrame_C();
    ImGui_NewFrame_C();

    showInformationImGUI(fps, deltaTime, stableFPS);

    ImGui_Render_C();
    ImGui_ImplOpenGL3_RenderDrawData_C();

    SDL_GL_SwapWindow(window);
  }

  cleanupWindow(window, glContext);

  return 0;
}
