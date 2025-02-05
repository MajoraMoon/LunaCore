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

  FrameTimer frameTimer;
  initFrameTimer(&frameTimer);

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

    updateFrameTimer(&frameTimer);

    renderFrame(&renderer, SCR_WIDTH, SCR_HEIGHT);

    // Dear ImGui set up
    ImGui_ImplOpenGL3_NewFrame_C();
    ImGui_ImplSDL3_NewFrame_C();
    ImGui_NewFrame_C();

    showInformationImGUI(frameTimer.fps, frameTimer.deltaTime,
                         frameTimer.stableFPS);

    ImGui_Render_C();
    ImGui_ImplOpenGL3_RenderDrawData_C();

    SDL_GL_SwapWindow(window);
  }

  cleanupRenderer(&renderer);
  cleanupWindow(window, glContext);

  return 0;
}

static void showInformationImGUI(float fps, float deltaTime, float stableFPS) {

  ImGui_Begin_C("Semi-useful Information");
  ImGui_Text_c("FPS (per frame): %1f", fps);
  ImGui_Text_c("Delta Time: %.5f", deltaTime);
  ImGui_Text_c("FPS (per second): %.1f", stableFPS);
  ImGui_VsyncCombo_C();
  ImGui_End_c();
}
