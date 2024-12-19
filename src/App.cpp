#include <vector>
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

    const std::vector<GLfloat> vertexPosition{

        // X    Y     Z
        -0.8f, -0.8f, 0.0f, // vertex 1
        0.8f, -0.8f, 0.0f,  // vertex 2
        0.0f, 0.8f, 0.0f    // vertex 3

    };

    VertexArrayObject vao;

    vao.bindVAO();

    VertexBufferObject vbo(vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    vao.setAttributePointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    vao.enableAttribute(0);

    vao.unbindVAO();
    vao.disableAttribute(0);

    Renderer renderer(window.getWindow(), window.getGLContext(), window);

    while (renderer.isRunning())
    {
        renderer.handleInputEvents();
        renderer.render();
    }

    return 0;
}
