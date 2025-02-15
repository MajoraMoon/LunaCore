#include "main.h"

static void showInformationImGUI(float fps, float deltaTime, float stableFPS);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main(int argc, char *argv[]) {

  // init SDL3 window with OpenGL context.
  SDL_Window *window =
      initWayWindowGL("LunaCore", "0.1", SCR_WIDTH, SCR_HEIGHT, true);

  if (window == NULL) {
    SDL_Log("Something went wrong in setting up a SDL window.\n");
    return -1;
  }

  SDL_GLContext glContext = initOpenGLContext_and_glad(window);

  if (!glContext) {
    SDL_Log("Something went wrong in setting up a glContext.\n");
    return -1;
  }

  initImGUI(window, glContext);

  Renderer renderer;
  initRenderer(&renderer);

  bool running = true;

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

    // here is the openGL rendering
    renderFrame(&renderer, SCR_WIDTH, SCR_HEIGHT);

    SDL_GL_SwapWindow(window);
  }

  cleanupRenderer(&renderer);
  cleanupWindow(window, glContext);

  return 0;
}
