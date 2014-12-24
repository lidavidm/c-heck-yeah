#include "entity.h"
#include <stdbool.h>

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct CollisionBox {
    int numVerticies;
    Position* verticies;
} Collisionbox;

typedef struct 

// provides an abstraction of a level
typedef struct Level  {
    int numTiles;

    int tileWidth; // in pixels
    int tileHeight;

    CollisionBox *boxes;
    // Add some representation of the tile sheet here

} Level;

// loads the given level into the world's entity fields
bool LoadLevel(World* world, Level level);
