#ifndef ENTITY
#define ENTITY
#include <chipmunk.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "defines.h"

typedef enum {
  NONE = 0,
  POSITION = 1 << 1, // cpBody's hold position information, but
  					 // I kept this just in case we need to
					 // track position in two coordinate systems 
  SPRITE = 1 << 2,
  HEALTH = 1 << 3,
  BODY = 1 << 4 // for physics
} EntityComponent; // renamed because of conflict with chipmunk

typedef struct Position {
  int x;
  int y;
} Position;

typedef struct Health {
  int health;
} Health;

typedef struct Sprite {
  // the size to render the sprite at
  int width;
  int height;

  SDL_Texture *texture;

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
  cpBody Body[ENTITY_COUNT];
  TTF_Font *font;
  SDL_Renderer *renderer;
  cpSpace *space;
} World;

void Position_New(World *world, int entity);
void Sprite_New(World *world, int entity,
                int width, int height,
                int frameWidth, int frameHeight, int frames);
void Sprite_NewFromTexture(World *world, int entity,
                           int width, int height,
                           SDL_Texture *texture,
                           int frameWidth, int frameHeight, int frames);
void Physics_Body_New(World *world, int entity);
void Text_New(World *world, int entity, char* text, SDL_Color color);

int Entity_New(World *world);

void Position_SetX(World *world, int entity, int x);
void Position_SetY(World *world, int entity, int y);
void Position_SetXY(World *world, int entity, int x, int y);
void World_Free(World *world);
void Entity_Free(World *world, int entity);
#endif
