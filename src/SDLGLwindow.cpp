#include "SDLGLwindow.h"

SDLGLwindow::SDLGLwindow(const std::string &title, GLuint width, GLuint height)
    : title(title), width(width), height(height), window(nullptr), glContext(nullptr)
{
}

SDLGLwindow::~SDLGLwindow()
{

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_Log("Quitted successfully");
}

bool SDLGLwindow::init()
{

    // First, initiate SDL. Here initiating sdl-subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initiate SDL: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initiate SDL-OpenGL-Window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)))
    {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    glContext = SDL_GL_CreateContext(window);

    if (!glContext)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Using the "Docking" branch of Dear ImGui.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable gamepad controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // who needs light mode
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(getSDLWindow(), getGLContext());

    ImGui_ImplOpenGL3_Init("#version 450");

    return true;
}

SDL_Window *SDLGLwindow::getSDLWindow() const
{
    return window;
}

SDL_GLContext SDLGLwindow::getGLContext() const
{
    return glContext;
}

GLuint SDLGLwindow::getInitWidth() const
{
    return width;
}

GLuint SDLGLwindow::getInitHeight() const
{
    return height;
}
