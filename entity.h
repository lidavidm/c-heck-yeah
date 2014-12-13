#ifndef ENTITY
#define ENTITY

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
  SDL_Texture texture;
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


int newEntity(World *world);
void drawSprites(World *world);

#endif
