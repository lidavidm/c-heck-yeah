#ifndef ENTITY
#define ENTITY

#include <SDL.h>
#include <SDL_image.h>

#define ENTITY_COUNT 2000

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

#endif
