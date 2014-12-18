#include "screens.h"

#include <SDL_ttf.h>

void initMain(World *world) {
  int entity = newEntity(world);
  newText(world, entity, "Title of the game");
}

void updateMain(World *world) {
}

void renderMain(World *world, SDL_Renderer *renderer) {
  for (int i = 0; i < ENTITY_COUNT; i++) {
    if (world->mask[i] & SPRITE) {
      SDL_RenderCopy(world->renderer, world->sprite[i].texture, NULL, NULL);
    }
  }
}

void updateLevel(World *world) {
}

void renderLevel(World *world, SDL_Renderer *renderer) {
}
