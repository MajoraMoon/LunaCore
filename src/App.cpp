#include <vector>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "SDLGLwindow.h"
#include "Renderer.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

int main(int argc, char *argv[])
{

    SDLGLwindow window("LunaCore", 1080, 720);

    if (!window.init())
    {
        SDL_Log("Failed to initialize the SDLGLwindow\n");

        return 1;
    }

    window.activateVsync(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable gamepad controls

#ifdef _WIN32

    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable docking branch

#endif

    ImGui::StyleColorsDark();

    // Frei schwebendes Fenster
    ImVec2 windowPos(200, 200);  // Setze die Position des schwebenden Fensters
    ImVec2 windowSize(400, 300); // Setze die Größe des Fensters

    // Setze das Fenster, bevor es gerendert wird
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);

    ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getGLContext());

    ImGui_ImplOpenGL3_Init("#version 450");

    Renderer renderer(window.getWindow(), window.getGLContext(), window);

    renderer.setup();

    while (renderer.isRunning())
    {
        renderer.handleInputEvents();
        renderer.render();
    }

    return 0;
}
