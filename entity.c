#include "entity.h"

#include <SDL_ttf.h>

/* Creates a new entity.
 *
 * Returns: index to the entity, or -1 if no more entities can be created.
 */
int Entity_New(World *world) {
  for (int i = 0; i < ENTITY_COUNT; i++) {
    if (world->mask[i] == NONE) {
      return i;
    }
  }
  return -1;
}

void Position_New(World *world, int entity) {
  world->mask[entity] |= POSITION;
  world->position[entity] = (Position) { 0, 0 };
}

void Sprite_New(World *world, int entity,
                int width, int height,
                int frameWidth, int frameHeight, int frames) {
  world->mask[entity] |= SPRITE;
  world->sprite[entity] = (Sprite) {
    width,
    height,
    NULL,
    frameWidth,
    frameHeight,
    frames,
    0,
    NULL
  };
  world->sprite[entity].frames = malloc(frames * sizeof(SDL_Rect));

  // precalculate rects for frames
  for (int i = 0; i < frames; i++) {
    world->sprite[entity].frames[i] = (SDL_Rect) {
      i * frameWidth,
      i * frameHeight,
      frameWidth,
      frameHeight
    };
  }
}

void Sprite_NewFromTexture(World *world, int entity,
                           int width, int height,
                           SDL_Texture *texture,
                           int frameWidth, int frameHeight, int frames) {
  Sprite_New(world, entity, width, height, frameWidth, frameHeight, frames);
  world->sprite[entity].texture = texture;
}

void Text_New(World *world, int entity, char* text, SDL_Color color) {
  SDL_Surface *surface = TTF_RenderText_Blended(world->font, text, color);

  if (surface == NULL) {
    printf("%d Couldn't render text! Error: %s\n", __LINE__, TTF_GetError());
    // end program
  }

  SDL_Texture *texture =
    SDL_CreateTextureFromSurface(world->renderer, surface);
  if (texture == NULL) {
    printf("Couldn't render text! Error: %s\n", SDL_GetError());
    // TODO: handle error
  }
  Sprite_New(world, entity, surface->w, surface->h, surface->w, surface->h, 1);
  world->sprite[entity].texture = texture;

  SDL_FreeSurface(surface);
}

void Position_SetX(World *world, int entity, int x) {
  world->position[entity].x = x;
}

void Position_SetY(World *world, int entity, int y) {
  world->position[entity].y = y;
}

void Position_SetXY(World *world, int entity, int x, int y) {
  world->position[entity].x = x;
  world->position[entity].y = y;
}

void World_Free(World *world) {
  TTF_CloseFont(world->font);
  SDL_DestroyRenderer(world->renderer);
  for (int i = 0; i < ENTITY_COUNT; i++) {
    if (world->mask[i] & SPRITE) {
      SDL_DestroyTexture(world->sprite[i].texture);
      free(world->sprite[i].frames);
    }
  }
  free(world);
}
