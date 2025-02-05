#ifndef IMGUI_C_H
#define IMGUI_C_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IMGuiIO {
  unsigned int ConfigFlags;
} IMGuiIO;

typedef struct IMGuiVec2 {
  float x, y;
} IMGuiVec2;

void ImGui_CreateContext();
void ImGui_SetupIO();
void ImGui_StyleColorsDark();
void ImGui_implSDL3_InitForOpenGL_C(void *window, void *context);
void ImGui_ImplOpenGL3_Init_C(const char *glsl_version);

void ImGui_ImplOpenGL3_NewFrame_C();
void ImGui_ImplSDL3_NewFrame_C();
void ImGui_NewFrame_C();
void ImGui_ShowDemoWindow();
void ImGui_Render_C();
void ImGui_ImplOpenGL3_RenderDrawData_C();
void ImGui_ImplSDL3_ProcessEvent_C(void *event);

#define ImGuiConfigFlags_NavEnableKeyboard 1
#define ImGuiConfigFlags_NavEnableGamepad 2
#define ImGuiConfigFlags_DockingEnable 4

#ifdef __cplusplus
}
#endif

#endif // IMGUI_C_H
