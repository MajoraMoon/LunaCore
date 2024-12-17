#include "SDLGL-window.h"

SDLGLwindow::SDLGLwindow(const std::string &title, GLuint width, GLuint height, int vsync)
    : title(title), width(width), vsync(vsync), window(nullptr) {}

SDLGLwindow::~SDLGLwindow()
{
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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initiate SDL: %s\n", SDL_GetError);
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

    return true;
}

// "1" will activate Vsync. Any other value will deactivate Vsync.
void SDLGLwindow::activateVsync(int vsync)
{
    if (vsync == 1)
    {
        SDL_GL_SetSwapInterval(1);
    }
    else
    {
        SDL_GL_SetSwapInterval(0);
    }
}

SDL_Window *SDLGLwindow::getWindow() const
{
    return window;
}

SDL_GLContext SDLGLwindow::getGLContext() const
{
    return glContext;
}