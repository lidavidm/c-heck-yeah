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
    PHYSICS = 1 << 4 // for physics
} EntityComponent; // renamed because of conflict with chipmunk

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Health {
    int health;
    int max;
} Health;

typedef struct SpriteAnimation {
    int start;
    int end;
} SpriteAnimation;

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

    int numAnimations;
    int curAnimation;
    SpriteAnimation *animations;
} Sprite;

typedef struct Physics {
    // Invariant: NULL iff shape is static shape attached to space->staticBody
    cpBody *body;
    cpShape *shape;

    int xOffset;
    int yOffset;
} Physics;

// http://stackoverflow.com/questions/10656588/why-shouldnt-i-use-pixels-as-unit-with-box2d
#define PIXELS_PER_METER 64.0

typedef struct World {
    int mask[ENTITY_COUNT];
    Position position[ENTITY_COUNT];
    Health health[ENTITY_COUNT];
    Sprite sprite[ENTITY_COUNT];
    Physics physics[ENTITY_COUNT];
    // TODO: move these to Game struct?
    TTF_Font *font;
    SDL_Renderer *renderer;
    cpSpace *space;
} World;

#define SPRITE_ANIMATION_NONE -1

void Position_New(World *world, int entity);
void Health_New(World *world, int entity, int health, int max);
SDL_Texture* Sprite_LoadTexture(World *world, char *path);
void Sprite_New(World *world, int entity,
                int width, int height,
                int frameWidth, int frameHeight, int frames,
                int animations);
void Sprite_NewFromTexture(World *world, int entity,
                           int width, int height,
                           SDL_Texture *texture,
                           int frameWidth, int frameHeight, int frames,
                           int animations);
void SpriteAnimation_New(World *world, int entity, int animation, int start, int end);
void Physics_New(World *world, int entity, cpBody *body, cpShape *shape,
                 int xoffset, int yoffset);
void Physics_NewStatic(World *world, int entity, cpShape *shape);
void Text_New(World *world, int entity, char* text, SDL_Color color);

int Entity_New(World *world);

int Position_GetX(World *world, int entity);
int Position_GetY(World *world, int entity);
void Position_SetX(World *world, int entity, int x);
void Position_SetY(World *world, int entity, int y);
void Position_SetXY(World *world, int entity, int x, int y);

void Health_Heal(World *world, int entity, int amount);
bool Health_Damage(World *world, int entity, int amount);
double Health_GetPercent(World *world, int entity);

bool Sprite_HitTest(World *world, int entity, int x, int y);
void Sprite_NextFrame(World *world, int entity);
int Sprite_GetFrame(World *world, int entity);
void Sprite_SetFrame(World *world, int entity, int frame);
void Sprite_SetAnimation(World *world, int entity, int animation);
void Sprite_StopAnimation(World *world, int entity);
void Sprite_Render(World *world, int entity);

// Update the position component based on the physics component
void Physics_SetPosition(World *world, int entity, cpFloat x, cpFloat y);
void Physics_UpdatePosition(World *world, int entity);
void Physics_Step(World *world, cpFloat step);
cpBody* Physics_GetBody(World *world, int entity);
cpShape* Physics_GetShape(World *world, int entity);

void World_Free(World *world);
void Entity_Free(World *world, int entity);
#endif
