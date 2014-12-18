#include "entity.h"

#include <SDL_ttf.h>

/* Creates a new entity.
 *
 * Returns: index to the entity, or -1 if no more entities can be created.
 */
int newEntity(World *world) {
  for (int i = 0; i < ENTITY_COUNT; i++) {
    if (world->mask[i] == NONE) {
      return i;
    }
  }
  return -1;
}

void newSprite(World *world, int entity, int frames) {
  world->mask[entity] |= SPRITE;
  Sprite *sprite = malloc(sizeof(Sprite) + frames * sizeof(SDL_Rect));
  world->sprite[entity] = *sprite;

  // TODO: precalculate rects for frames
}

void newText(World *world, int entity, char* text) {
  SDL_Surface *surface = TTF_RenderText_Solid(world->font, text, (SDL_Color) { 255, 255, 255, 255 });

  if (surface == NULL) {
    printf("%d Couldn't render text! Error: %s\n", __LINE__, TTF_GetError());
    // end program
  }

  newSprite(world, entity, 1);
  world->sprite[entity].texture =
    SDL_CreateTextureFromSurface(world->renderer, surface);
  if (world->sprite[entity].texture == NULL) {
    printf("Couldn't render text! Error: %s\n", SDL_GetError());
    // TODO: handle error
  }
  world->sprite[entity].textureWidth = world->sprite[entity].frameWidth = surface->w;
  world->sprite[entity].textureHeight = world->sprite[entity].frameHeight = surface->h;

  SDL_FreeSurface(surface);
}
