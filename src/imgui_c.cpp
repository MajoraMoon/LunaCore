// clang-format off
#include <cstdarg>
#include <cstdio>   
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "imgui_c.h"
#include <SDL3/SDL.h>

// clang-format on

typedef enum { VSyncOFF = 0, VSyncON = 1 } VsyncMode;

static VsyncMode currentVsyncMode = VSyncOFF;

static void activateVsync(VsyncMode mode) {

  if (!SDL_GL_SetSwapInterval((int)mode)) {
    SDL_Log("Could not set Swap-Interval (VSync) correctly: %s\n",
            SDL_GetError());
  } else {
    currentVsyncMode = mode;
  }
}

// init imgui
void ImGui_CreateContext_C() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
}

void ImGui_SetupIO_C() {
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

// dark mode all the way
void ImGui_StyleColorsDark_C() { ImGui::StyleColorsDark(); }

void ImGui_implSDL3_InitForOpenGL_C(void *window, void *context) {
  ImGui_ImplSDL3_InitForOpenGL((SDL_Window *)window, (SDL_GLContext)context);
}

void ImGui_ImplOpenGL3_Init_C(const char *glsl_version) {
  ImGui_ImplOpenGL3_Init(glsl_version);
}

// functions for render loop
void ImGui_ImplOpenGL3_NewFrame_C() { ImGui_ImplOpenGL3_NewFrame(); }
void ImGui_ImplSDL3_NewFrame_C() { ImGui_ImplSDL3_NewFrame(); }
void ImGui_NewFrame_C() { ImGui::NewFrame(); }
void ImGui_Render_C() { ImGui::Render(); }
void ImGui_ImplOpenGL3_RenderDrawData_C() {
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// for input events with SDl3
void ImGui_ImplSDL3_ProcessEvent_C(void *event) {
  ImGui_ImplSDL3_ProcessEvent((SDL_Event *)event);
}

// demo window from Dear ImGUI itself
void ImGui_ShowDemoWindow_C() { ImGui::ShowDemoWindow(); }

// window decorations and set up
void ImGui_Begin_C(const char *window_information) {
  ImGui::Begin(window_information);
}

void ImGui_Text_c(const char *format, ...) {
  va_list args;
  va_start(args, format);

  va_list args_copy;
  va_copy(args_copy, args);

  int required = vsnprintf(nullptr, 0, format, args_copy);
  va_end(args_copy);

  if (required < 0) {
    va_end(args);
    return;
  }

  int bufferSize = required + 1;
  char *buffer = new char[bufferSize];

  vsnprintf(buffer, bufferSize, format, args);

  va_end(args);

  ImGui::Text("%s", buffer);

  delete[] buffer;
}

extern "C" void ImGui_VsyncCombo_C() {
  const char *vsyncOptions[] = {"Vsync Off", "Vsync On"};
  int currentVSyncIndex = static_cast<int>(currentVsyncMode);

  if (ImGui::Combo("VSync Mode", &currentVSyncIndex, vsyncOptions,
                   IM_ARRAYSIZE(vsyncOptions))) {
    currentVsyncMode = static_cast<VsyncMode>(currentVSyncIndex);
    activateVsync(currentVsyncMode);
  }
}

void ImGui_End_c() { ImGui::End(); }