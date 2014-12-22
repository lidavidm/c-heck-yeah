#include "defines.h"
#include "screens.h"

#include <SDL.h>
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
bool Main_Init(World *world, void **state) {
  bool error = false;
  int bg, title, continueGame, quit;
  SDL_Surface *surface = NULL;
  SDL_Texture *texture = NULL;

  bg = Entity_New(world);
  Position_New(world, bg);
  Position_SetXY(world, bg, 0, 0);
  surface = IMG_Load("bg.png");
  if (surface == NULL) {
    printf("%d Could not load background image: %s\n", __LINE__, IMG_GetError());
    goto error;
  }
  texture = SDL_CreateTextureFromSurface(world->renderer, surface);
  if (texture == NULL) {
    printf("%d Could not load create background texture: %s\n", __LINE__, SDL_GetError());
    goto error;
  }

  Sprite_NewFromTexture(world, bg, WINDOW_WIDTH, WINDOW_HEIGHT, texture, surface->w, surface->h, 1);

  title = Entity_New(world);
  Position_New(world, title);
  Position_SetXY(world, title, 720, 151);
  Text_New(world, title, "Magical Girl Michael Mauer", (SDL_Color) { 200, 0, 0, 200 });

  continueGame = Entity_New(world);
  Position_New(world, continueGame);
  Position_SetXY(world, continueGame, 720, 250);
  Text_New(world, continueGame, "Continue", (SDL_Color) { 0, 0, 0, 200 });

  quit = Entity_New(world);
  Position_New(world, quit);
  Position_SetXY(world, quit, 720, 350);
  Text_New(world, quit, "Quit", (SDL_Color) { 0, 0, 0, 200 });

  *state = malloc(sizeof(MainState));
  ((MainState*) *state)->continueEntity = continueGame;
  ((MainState*) *state)->quitEntity = quit;

  goto cleanup;
 error:
  error = true;
 cleanup:
  SDL_FreeSurface(surface);

  return !error;
}

void Main_Update(World *world, void *state) {
}

void Main_HandleEvent(World *world, SDL_Event *event, void *state) {
  if (event->type == SDL_MOUSEBUTTONUP) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (Sprite_HitTest(world, ((MainState*) state)->continueEntity, x, y)) {
      printf("%s\n", "Clicked continue");
    }
  }
}

void Main_Render(World *world, void *state) {
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

void Level_HandleEvent(World *world, SDL_Event *event, void *state) {
}

void Level_Update(World *world, void *state) {
}

void Level_Render(World *world, void *state) {
}
