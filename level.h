#include "entity.h"

typedef struct CollisionBoxCoord {
    int x;
    int y;
} CollisionBoxCoord;

typedef struct CollisionBox {
    int numVerticies;
    CollisionBoxCoord* verticies;
} Collisionbox;

// provides an abstraction of a level
typedef struct TileSet  {
    int numTiles;
    int tileWidth; // in pixels
    int tileHeight;
    CollisionBox *boxes;
    // Add some representation of the tile sheet here
} TileSet;

typedef struct TileCoord  {
    int x;
    int y;
    int tileID; // must be consistent with whatever tile set is being used!
} TileCoord;

// specifies a rectangular world, starting from (0,0) going to
// (x_length, y_length), exclusively
typedef struct Level  {
    TileSet *tileset;
    int x_length;
    int y_length;
    TileCoord **tileMatrix;
} Level;

// Loads the given level into the world's entity list
bool loadLevel(World *world, Level level);

// Functions for level resources. Return NULL if loading fails.
// TODO: Potentially add better error handling for this (error messages?)
Level* loadLevelFromFile(char* filePath);
TileSet* loadTileSetFromDirectory(char* directoryName);
