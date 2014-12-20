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

void newPosition(World *world, int entity) {
  world->mask[entity] |= POSITION;
  world->position[entity] = (Position) { 0, 0 };
}

void newSprite(World *world, int entity,
               int textureWidth, int textureHeight,
               int frameWidth, int frameHeight, int frames) {
  world->mask[entity] |= SPRITE;
  Sprite *sprite = malloc(sizeof(Sprite) + frames * sizeof(SDL_Rect));
  sprite->curFrame = 0;
  sprite->numFrames = frames;
  sprite->textureWidth = textureWidth;
  sprite->textureHeight = textureHeight;
  sprite->frameWidth = frameWidth;
  sprite->frameHeight = frameHeight;
  world->sprite[entity] = *sprite;

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

void newText(World *world, int entity, char* text) {
  SDL_Surface *surface = TTF_RenderText_Blended(world->font, text, (SDL_Color) { 255, 255, 255, 255 });

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
  newSprite(world, entity, surface->w, surface->h, surface->w, surface->h, 1);
  world->sprite[entity].texture = texture;

  SDL_FreeSurface(surface);
}

void setPositionX(World *world, int entity, int x) {
  world->position[entity].x = x;
}

void setPositionY(World *world, int entity, int y) {
  world->position[entity].y = y;
}

void setPosition(World *world, int entity, int x, int y) {
  world->position[entity].x = x;
  world->position[entity].y = y;
}
