#ifndef WORLD
#define WORLD

#include "entity.h"

typedef struct World {
  int mask[ENTITY_COUNT];

  Health health[ENTITY_COUNT];
  Sprite sprite[ENTITY_COUNT];
} World;


int newEntity(World *world);
void drawSprites(World *world);

#endif
