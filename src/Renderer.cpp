#include <Renderer.h>

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

// Creating the inital Render Object is turning vsync off as default. It can be turned on in the Dear Imgui panel.
Renderer::Renderer(SDL_Window *window, SDL_GLContext glContext, SDLGLwindow &sdgl_window)
    : window(window), glContext(glContext), running(true), sdgl_window(sdgl_window), currentVsyncMode(VSync_Off)
{
    activateVsync(currentVsyncMode);
}

// setting up all vertex and array object before starting the actual rendering
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

    // going through all corners of the rectangles, starting counting from 0.
    const std::vector<GLuint> elementBufferData{0, 1, 2, 1, 3, 2};

    // later maybe making it work on the stack instead on the heap although it is working with smart pointers...
    // all these objects are creating *one* vao or vbo (whatever) e.g. glGenBuffers(1, &vboID);
    vao = std::make_unique<VertexArrayObject>();

    vao->bindVAO();

    // vbo is already binded when calling the constuctors
    vbo = std::make_unique<VertexBufferObject>(vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    ebo = std::make_unique<ElementBufferObject>();

    // Pointing to the vertices
    vao->setAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)0);

    vao->enableAttribute(0);
    // Pointing to the rgb values
    vao->setAttributePointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)(sizeof(GLfloat) * 3));

    vao->enableAttribute(1);

    ebo->bindEBO();

    ebo->bufferData(GL_ELEMENT_ARRAY_BUFFER, elementBufferData.size() * sizeof(GLuint), elementBufferData.data(), GL_STATIC_DRAW);

    // Pointing to the coords of the texture. With stride 8 and offset 6.
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    delete[] imgData;
}

void Renderer::render()
{
    // little logic , so the "SDL_GL_SetSwapInterval()" won't be called every frame. It will be called once when the vsync settings changed.
    if (currentVsyncMode != lastVsyncMode)
    {
        activateVsync(currentVsyncMode);
        lastVsyncMode = currentVsyncMode;
    }

    // calculating delta time and the Frame Rate per second. the "StableFPS" is putting out the fps value every new second. so it's better to see the average value.
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

    // Dear ImGui set up
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

    showInformationImGui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
}

void Renderer::handleInputEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

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

// just returns a bool for the running renderer. The constructor is setting the bool to "true" as default. Otherwise this all here would not work lol
bool Renderer::isRunning() const
{
    return running;
}

// adding the "adaptive Vsync" setting from SDL2 is crashing the applicaton for some reason. removed it because of that.
void Renderer::activateVsync(VsyncMode vsync)
{

    switch (vsync)
    {
    case VSync_On:
        SDL_GL_SetSwapInterval(1);
        break;
    case VSync_Off:
        SDL_GL_SetSwapInterval(0);
        break;
    default:
        throw std::invalid_argument("You usually cannot get this error message.\n If you did tho, you fucked up the vsync setting somehow, good job!");
        break;
    }
}

// Just render useful information in a simple Dear ImGui window.
void Renderer::showInformationImGui()
{

    ImGui::Begin("Useful Information");
    ImGui::Text("FPS (per frame): %.1f", fps);
    ImGui::Text("Delta Time: %.5f", deltaTime);
    ImGui::Text("FPS (per second): %.1f", stableFPS);

    const char *vsyncOptions[] = {"Vsync Off", "Vsync On"};
    int currentVSyncIndex = static_cast<int>(currentVsyncMode);

    if (ImGui::Combo("VSync Mode", &currentVSyncIndex, vsyncOptions, IM_ARRAYSIZE(vsyncOptions)))
    {
        currentVsyncMode = static_cast<VsyncMode>(currentVSyncIndex);
        activateVsync(currentVsyncMode);
    }

    ImGui::End();
}
