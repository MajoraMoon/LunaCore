#include <Renderer.h>

void Renderer::showFPSWindow()
{

    ImGui::Begin("Performance");
    ImGui::Text("FPS (per frame): %.1f", fps);
    ImGui::Text("Delta Time: %.4f", deltaTime);
    ImGui::Text("FPS (per second): %.1f", stableFPS);
    ImGui::End();
}

unsigned char *loadImage(const char *filePath, int &width, int &height, int &nrChannels)
{

    SDL_Surface *surface = IMG_Load(filePath);

    if (!surface)
    {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    width = surface->w;
    height = surface->h;

    nrChannels = surface->format->BytesPerPixel;

    unsigned char *byteDataImg = new unsigned char[width * height * nrChannels];
    memcpy(byteDataImg, surface->pixels, width * height * nrChannels);

    SDL_FreeSurface(surface);

    return byteDataImg;
}

Renderer::Renderer(SDL_Window *window, SDL_GLContext glContext, SDLGLwindow &sdgl_window)
    : window(window), glContext(glContext), running(true), sdgl_window(sdgl_window)
{
}

void Renderer::setup()
{
    shader = std::make_unique<Shader>("../shader/vertexShader.vert", "../shader/fragmentShader.frag");

    const std::vector<GLfloat> vertexPosition{

        // positions        //colors         // texure coords
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // vertex 0 - bottom left position and rgb color - red
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // vertex 1 - bottom right position and rgb color - green
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // vertex 2 - top left position and rgb color - blue
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f    // vertex 3 - top right position and rgb color - red

    };

    const std::vector<GLuint> elementBufferData{0, 1, 2, 1, 3, 2};

    vao = std::make_unique<VertexArrayObject>();

    vao->bindVAO();

    vbo = std::make_unique<VertexBufferObject>(vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    ebo = std::make_unique<ElementBufferObject>();

    vao->setAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)0);

    vao->enableAttribute(0);
    // rgb
    vao->setAttributePointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)(sizeof(GLfloat) * 3));

    vao->enableAttribute(1);

    ebo->bindEBO();

    ebo->bufferData(GL_ELEMENT_ARRAY_BUFFER, elementBufferData.size() * sizeof(GLuint), elementBufferData.data(), GL_STATIC_DRAW);

    // Points the index 2 in the vao to the texture coordsi in the vbo. with stride 8 and offset 6.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)(sizeof(GLfloat) * 6));
    glEnableVertexAttribArray(2);

    // load and create a texture
    glGenTextures(1, &texture);
    // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLsizei width, height, nrChannels;

    unsigned char *imgData = loadImage("../assets/textures/rocky_terrain_diff_4k.jpg", width, height, nrChannels);

    if (imgData)
    {
        std::cout << "Image loaded successfully." << std::endl;
        std::cout << "Width: " << width << ", Height: " << height << ", Channels " << nrChannels << std::endl;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else
    {
        std::cout << "Failed to loa texture" << std::endl;
        SDL_Log("hello");
    }

    delete[] imgData;
}

void Renderer::render()
{

    float currentFrame = SDL_GetTicks() / 1000.0f;

    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    fps = 1.0f / deltaTime;

    timeAccumulator += deltaTime;
    frameCount++;

    if (timeAccumulator >= 1.0f)
    {
        stableFPS = frameCount / timeAccumulator;
        frameCount = 0;
        timeAccumulator = 0.0f;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    glViewport(0, 0, sdgl_window.getInitWidth(), sdgl_window.getInitHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);

    shader->useShaderProgram();

    vao->bindVAO();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    showFPSWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef _WIN32

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

#endif
    SDL_GL_SwapWindow(window);
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

        ImGui_ImplSDL2_ProcessEvent(&event);
    }
}

bool Renderer::isRunning() const
{
    return running;
}
