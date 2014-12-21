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
void Main_Init(World *world) {
  int entity = Entity_New(world);
  Position_New(world, entity);
  Position_SetXY(world, entity, 50, 211);
  Text_New(world, entity, "Magical Girl Michael Mauer", (SDL_Color) { 255, 0, 0, 255 });
}

void Main_Update(World *world) {
}

void Main_Render(World *world, SDL_Renderer *renderer) {
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

void Level_Update(World *world) {
}

void Level_Render(World *world, SDL_Renderer *renderer) {
}
