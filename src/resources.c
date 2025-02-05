#include <resources.h>

unsigned char *loadImage(const char *filePath, int *width, int *height) {
  SDL_Surface *surface = IMG_Load(filePath);

  if (!surface) {
    fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
    return NULL;
  }

  *width = surface->w;
  *height = surface->h;

  // Berechne die Datenmenge – Achtung:
  // Häufig muss hier auch die Anzahl der Kanäle (Bytes per Pixel)
  // berücksichtigt werden!
  size_t dataSize =
      (*width) * (*height) * (SDL_BITSPERPIXEL(surface->format) / 8);
  unsigned char *byteDataImg = malloc(dataSize);
  if (!byteDataImg) {
    fprintf(stderr, "Memory allocation failed.\n");
    SDL_DestroySurface(surface);
    return NULL;
  }

  memcpy(byteDataImg, surface->pixels, dataSize);
  SDL_DestroySurface(surface);

  return byteDataImg;
}