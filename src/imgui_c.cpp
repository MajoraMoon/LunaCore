// clang-format off
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "imgui_c.h"
#include <SDL3/SDL.h>

// clang-format on
void ImGui_CreateContext() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
}

void ImGui_SetupIO() {
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;            // Enable gamepad controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
}

void ImGui_StyleColorsDark() { ImGui::StyleColorsDark(); }

void ImGui_implSDL3_InitForOpenGL(void *window, void *context) {
  ImGui_ImplSDL3_InitForOpenGL((SDL_Window *)window, (SDL_GLContext)context);
}

void ImGui_ImplOpenGL3_Init_(const char *glsl_version) {
  ImGui_ImplOpenGL3_Init(glsl_version);
}