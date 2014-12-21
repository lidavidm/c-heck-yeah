#ifndef ENTITY
#define ENTITY

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "defines.h"

typedef enum {
  NONE = 0,
  POSITION = 1 << 1,
  SPRITE = 1 << 2,
  HEALTH = 1 << 3
} Component;

typedef struct Position {
  int x;
  int y;
} Position;

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
  int curFrame;
  SDL_Rect *frames;
} Sprite;

typedef struct Text {
} Text;

typedef struct World {
  int mask[ENTITY_COUNT];

  Position position[ENTITY_COUNT];
  Health health[ENTITY_COUNT];
  Sprite sprite[ENTITY_COUNT];

  TTF_Font *font;
  SDL_Renderer *renderer;
} World;

void Position_New(World *world, int entity);
void Sprite_New(World *world, int entity,
               int textureWidth, int textureHeight,
               int frameWidth, int frameHeight, int frames);
void Text_New(World *world, int entity, char* text, SDL_Color color);

int Entity_New(World *world);

void Position_SetX(World *world, int entity, int x);
void Position_SetY(World *world, int entity, int y);
void Position_SetXY(World *world, int entity, int x, int y);

void World_Free(World *world);

#endif
