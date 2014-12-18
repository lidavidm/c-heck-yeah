#ifndef SCREENS
#define SCREENS

#include <SDL.h>

#include "entity.h"

typedef enum {
  SCREEN_MAIN,
  SCREEN_LEVEL,
  SCREEN_VICTORY,
  SCREEN_DEFEAT,
  SCREEN_LOADING,
  SCREEN_CREDITS
} Screen;

void initMain(World *world);
void updateMain(World *world);
void renderMain(World *world, SDL_Renderer *renderer);

void updateLevel(World *world);
void renderLevel(World *world, SDL_Renderer *renderer);

#endif
