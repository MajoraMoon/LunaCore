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

void ImGui_CreateContext_C();
void ImGui_SetupIO_C();
void ImGui_StyleColorsDark_C();
void ImGui_implSDL3_InitForOpenGL_C(void *window, void *context);
void ImGui_ImplOpenGL3_Init_C(const char *glsl_version);

void ImGui_ImplOpenGL3_NewFrame_C();
void ImGui_ImplSDL3_NewFrame_C();
void ImGui_NewFrame_C();
void ImGui_Render_C();
void ImGui_ImplOpenGL3_RenderDrawData_C();

void ImGui_ImplSDL3_ProcessEvent_C(void *event);

void ImGui_ShowDemoWindow_C();

void ImGui_Begin_C(const char *window_information);
void ImGui_End_c();
void ImGui_Text_c(const char *format, ...);

void ImGui_VsyncCombo_C();

#ifdef __cplusplus
}
#endif

#endif // IMGUI_C_H
