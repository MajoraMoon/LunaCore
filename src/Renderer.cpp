#include <Renderer.h>

Renderer::Renderer(SDL_Window *window, SDL_GLContext glContext, SDLGLwindow &sdgl_window)
    : window(window), glContext(glContext), running(true), sdgl_window(sdgl_window) {}

void Renderer::setup()
{
    shader = std::make_unique<Shader>("../shader/vertexShader.vert", "../shader/fragmentShader.frag");

    const std::vector<GLfloat> vertexPosition{

        // X    Y     Z
        -0.8f, -0.8f, 0.0f, // vertex 1
        0.8f, -0.8f, 0.0f,  // vertex 2
        0.0f, 0.8f, 0.0f    // vertex 3

    };

    vao = std::make_unique<VertexArrayObject>();

    vao->bindVAO();

    vbo = std::make_unique<VertexBufferObject>(vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    vao->setAttributePointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    vao->enableAttribute(0);

    // vao->unbindVAO();
    //  vao->disableAttribute(0);
}

void Renderer::render()
{
    glViewport(0, 0, sdgl_window.getInitWidth(), sdgl_window.getInitHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->useShaderProgram();

    vao->bindVAO();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window);

    OutPut_fps();
}

void Renderer::handleInputEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        {
            running = false;
        }
    }
}

bool Renderer::isRunning() const
{
    return running;
}
