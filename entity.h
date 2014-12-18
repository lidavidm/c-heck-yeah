#ifndef ENTITY
#define ENTITY

#include <SDL.h>
#include <SDL_image.h>

#include "defines.h"

typedef enum {
  NONE = 0,
  HEALTH = 1 << 0,
  SPRITE = 1 << 1
} Component;

typedef struct Health {
  int health;
} Health;

typedef struct Sprite {
  SDL_Texture *texture;
  int textureWidth;
  int textureHeight;

  int frameWidth;
  int frameHeight;

  int numFrames;
  SDL_Rect *frames;
} Sprite;

typedef struct World {
  int mask[ENTITY_COUNT];

  Health health[ENTITY_COUNT];
  Sprite sprite[ENTITY_COUNT];
} World;

void newSprite(World *world, int entity);


int newEntity(World *world);
void drawSprites(World *world);

#endif
