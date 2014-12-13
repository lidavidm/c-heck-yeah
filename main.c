#include "entity.h"

#include <stdbool.h>
#include <SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(void) {
  SDL_Window *window = NULL;
  SDL_Surface *surface = NULL;
  bool quit = false;
  SDL_Event evt;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Couldn't initialize SDL. Error: %s\n", SDL_GetError());
    goto cleanup;
  }

  window = SDL_CreateWindow("Michael and the Hex World",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WINDOW_WIDTH,
                            WINDOW_HEIGHT,
                            SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Couldn't create window. Error: %s\n", SDL_GetError());
    goto cleanup;
  }

  surface = SDL_GetWindowSurface(window);

  while (!quit) {
    while (SDL_PollEvent(&evt) != 0) {
      if (evt.type == SDL_QUIT) {
        quit = true;
      }
    }

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);
    SDL_Delay(200);
  }

 cleanup:
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
