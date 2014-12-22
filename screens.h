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

typedef struct {
  int continueEntity;
  int quitEntity;
} MainState;

bool Main_Init(World *world, void **state);
void Main_Update(World *world, void *state);
void Main_HandleEvent(World *world, SDL_Event *event, void *state);
void Main_Render(World *world, void *state);
// void Main_End

void Level_Update(World *world, void *state);
void Level_HandleEvent(World *world, SDL_Event *event, void *state);
void Level_Render(World *world, void *state);

#endif
