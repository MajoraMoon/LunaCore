#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <freetype/freetype.h>

#include "fps.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
bool quit = false;
float fps = 0;

void RenderText(SDL_Renderer *renderer, FT_Face face, const char *text, int x, int y)
{
    // Set initial drawing position
    int pen_x = x;
    int pen_y = y;

    // Going through every single character
    for (const char *p = text; *p; p++)
    {
        // Loa character glyph (char) into FreeType
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
        {
            fprintf(stderr, "Failed to load glyph for character: %c\n", *p);
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load glyph for character: %c\n", *p);
            continue;
        }
        // Access the glyph's bitmap
        FT_Bitmap *bitmap = &face->glyph->bitmap;

        // Create SDL surface from FreeType bitmap
        SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(
            bitmap->buffer,
            bitmap->width,
            bitmap->rows,
            8,             // Bits per Pixel
            bitmap->pitch, // row size
            SDL_PIXELFORMAT_INDEX8);

        // Map colors -> white text
        SDL_Color colors[256] = {0};
        colors[255].r = 255;
        colors[255].g = 255;
        colors[255].b = 255;
        colors[255].a = 255;
        SDL_SetPaletteColors(surface->format->palette, colors, 0, 256);

        // Convert surface to texture
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        // Set destination recangle on the screen
        SDL_Rect dstrect = {
            pen_x + face->glyph->bitmap_left,
            pen_y - face->glyph->bitmap_top,
            bitmap->width,
            bitmap->rows};

        // Render texture to the screen
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        // Clean up texture
        SDL_DestroyTexture(texture);

        // Advance pen position
        pen_x += face->glyph->advance.x >> 6;
    }
}

int main(int argc, char *argv[])
{
    // First, initiate SDL. Here initiating sdl-subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initiate SDL: %s\n", SDL_GetError);
        return 1;
    }

    // Create Window instance
    SDL_Window *window = SDL_CreateWindow("LunaCore",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCR_WIDTH,
                                          SCR_HEIGHT,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initiate SDL-Window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Renderer uses hardware acceleration and vsync.
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Could not create SDL-Renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initiate FreeType to Render Fonts
    FT_Library ft_lib;
    if (FT_Init_FreeType(&ft_lib))
    {
        fprintf(stderr, "Could not initialize FreeType library\n");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize FreeType library\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load a Font with FreeType
    FT_Face ft_face;
    if (FT_New_Face(ft_lib, "../assets/fonts/Roboto/Roboto-Regular.ttf", 0, &ft_face))
    {
        fprintf(stderr, "Could not load font\n");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load font\n");
        FT_Done_FreeType(ft_lib);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set font size
    FT_Set_Pixel_Sizes(ft_face, 0, 48);

    SDL_Event event;

    // activates V-sync. Similar to the GLFW function "glfwSwapInterval(int a)"
    // SDL_GL_SetSwapInterval(1);

    // main loop. Different than in OpenGL itself, the window does not needs to be updated every frame
    // The Main window is created until SDL_QUIT is set to true...
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            // exit the main loop and closes the window
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }

            // Detects if the key below is pressed down
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = true;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render Text
        RenderText(renderer, ft_face, "Hello World!", 50, 100);

        SDL_RenderPresent(renderer);

        fps = calculateFPS_Float();
        SDL_Log("FPS: %.2f\n", fps);
    }

    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_lib);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_Log("Quitted successfully");

    return 0;
}
