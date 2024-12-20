#include <Renderer.h>

Renderer::Renderer(SDL_Window *window, SDL_GLContext glContext, SDLGLwindow &sdgl_window)
    : window(window), glContext(glContext), running(true), sdgl_window(sdgl_window) {}

void Renderer::setup()
{
    shader = std::make_unique<Shader>("../shader/vertexShader.vert", "../shader/fragmentShader.frag");

    const std::vector<GLfloat> vertexPosition{

        // X    Y     Z
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // vertex 0 - bottom left position and rgb color - red
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // vertex 1 - bottom right position and rgb color - green
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // vertex 2 - top left position and rgb color - blue

        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // vertex 3 - top right position and rgb color - green

    };

    const std::vector<GLuint> elementBufferData{2, 0, 1, 3, 2, 1};

    vao = std::make_unique<VertexArrayObject>();

    vao->bindVAO();

    vbo = std::make_unique<VertexBufferObject>(vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    ebo = std::make_unique<ElementBufferObject>();

    vao->setAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid *)0);

    vao->enableAttribute(0);
    // rgb
    vao->setAttributePointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid *)(sizeof(GLfloat) * 3));

    vao->enableAttribute(1);

    ebo->bindEBO();

    ebo->bufferData(GL_ELEMENT_ARRAY_BUFFER, elementBufferData.size() * sizeof(GLuint), elementBufferData.data(), GL_STATIC_DRAW);
}

void Renderer::render()
{
    glViewport(0, 0, sdgl_window.getInitWidth(), sdgl_window.getInitHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->useShaderProgram();

    vao->bindVAO();

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
