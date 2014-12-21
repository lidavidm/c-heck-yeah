#include "entity.h"
#include "screens.h"

#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define TICKS_PER_UPDATE 25
// 25 ms/update = 40 updates/s

int main(void) {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  bool quit = false;
  SDL_Event evt;
  unsigned int lastTime = 0;
  unsigned int lag = 0;
  Screen screen = SCREEN_MAIN;
  World *world = malloc(sizeof(World));

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

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("Couldn't create renderer. Error: %s\n", SDL_GetError());
    goto cleanup;
  }
  world->renderer = renderer;

  if (TTF_Init() != 0) {
    printf("Couldn't load font renderer. Error: %s\n", TTF_GetError());
    goto cleanup;
  }

  SDL_RWops *fontFile = SDL_RWFromFile("nsans.ttf", "rb");
  if (fontFile == NULL) {
    printf("%d Couldn't load font. Error: %s\n", __LINE__, SDL_GetError());
    goto cleanup;
  }
  world->font = TTF_OpenFontRW(fontFile, 1, 42);
  if (world->font == NULL) {
    printf("%d Couldn't load font. Error: %s\n", __LINE__, TTF_GetError());
    goto cleanup;
  }

  lastTime = SDL_GetTicks();
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  Main_Init(world);

  while (!quit) {
    unsigned int currentTime = SDL_GetTicks();
    unsigned int delta = currentTime - lastTime;
    lastTime = currentTime;
    lag += delta;

    // TODO: store this info and pass to update
    while (SDL_PollEvent(&evt) != 0) {
      if (evt.type == SDL_QUIT) {
        quit = true;
      }
    }

    while (lag >= TICKS_PER_UPDATE) {
      switch (screen) {
      case SCREEN_MAIN:
        Main_Update(world);
        break;
      case SCREEN_LEVEL:
        Level_Update(world);
        break;
      default:
        break;
      }
      lag -= TICKS_PER_UPDATE;
    }

    SDL_RenderClear(renderer);
    switch (screen) {
    case SCREEN_MAIN:
      Main_Render(world, renderer);
      break;
    case SCREEN_LEVEL:
      Level_Render(world, renderer);
      break;
    default:
      break;
    }
    SDL_RenderPresent(renderer);

    if (delta < 33) {
      SDL_Delay(33 - delta);
    }
  }

 cleanup:
  World_Free(world);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
