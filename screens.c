#include "screens.h"

#include <SDL_ttf.h>
#include <SDL_image.h>

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
  int bg = Entity_New(world);
  Position_New(world, bg);
  Position_SetXY(world, bg, 0, 0);
  SDL_Surface *surface = IMG_Load("bg.png");
  if (surface == NULL) {
    printf("%d Could not load background image: %s\n", __LINE__, IMG_GetError());
    // TODO: error handling
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(world->renderer, surface);
  if (texture == NULL) {
    printf("%d Could not load create background texture: %s\n", __LINE__, SDL_GetError());
    // TODO: error handling
  }
  // TODO: don't hardcode screen size
  Sprite_NewFromTexture(world, bg, 1280, 720, texture, surface->w, surface->h, 1);

  int title = Entity_New(world);
  Position_New(world, title);
  Position_SetXY(world, title, 720, 151);
  Text_New(world, title, "Magical Girl Michael Mauer", (SDL_Color) { 0, 0, 0, 200 });

  int continueGame = Entity_New(world);
  Position_New(world, continueGame);
  Position_SetXY(world, continueGame, 720, 250);
  Text_New(world, continueGame, "Continue", (SDL_Color) { 0, 0, 0, 200 });

  int quit = Entity_New(world);
  Position_New(world, quit);
  Position_SetXY(world, quit, 720, 350);
  Text_New(world, quit, "Quit", (SDL_Color) { 0, 0, 0, 200 });

  // cleanup:
  SDL_FreeSurface(surface);
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
