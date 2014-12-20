#include "screens.h"

#include <SDL_ttf.h>

/*
  TODO: this needs some hardcore refactoring + design work

  - Each screen needs some place to store its state. May need to pass a void
    pointer around

  - Rendering: we need to handle z-ordering somehow. Also scanning every
    entity on every frame is rather wasteful

    For rendering: have a separate renderer that is given pointers to the
    relevant components
 */
void initMain(World *world) {
  int entity = newEntity(world);
  newPosition(world, entity);
  setPosition(world, entity, 50, 211);
  newText(world, entity, "Magical Girl Michael Mauer");
}

void updateMain(World *world) {
}

void renderMain(World *world, SDL_Renderer *renderer) {
  for (int i = 0; i < ENTITY_COUNT; i++) {
    if (world->mask[i] & SPRITE) {
      int frame = world->sprite[i].curFrame;
      Position position = world->position[i];
      Sprite sprite = world->sprite[i];
      SDL_Rect target = {
        position.x,
        position.y,
        sprite.frameWidth,
        sprite.frameHeight,
      };
      SDL_RenderCopy(world->renderer, sprite.texture,
                     &world->sprite[i].frames[frame],
                     &target);
    }
  }
}

void updateLevel(World *world) {
}

void renderLevel(World *world, SDL_Renderer *renderer) {
}
