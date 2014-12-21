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

void Main_Init(World *world);
void Main_Update(World *world);
void Main_Render(World *world, SDL_Renderer *renderer);

void Level_Update(World *world);
void Level_Render(World *world, SDL_Renderer *renderer);

#endif
