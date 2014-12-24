#include "entity.h"

#include <SDL_ttf.h>

/* Creates a new entity.
 * Potential TODO: Devise better algorithm to find new entity number
 * Returns: index to the entity, or -1 if no more entities can be created.
 */
int Entity_New(World *world) {
    for (int i = 0; i < ENTITY_COUNT; i++) {
        if (world->mask[i] == NONE) {
            return i;
        }
    }
    return -1;
}

// Creates a default position (e.g: 0,0) for the given entity
void Position_New(World *world, int entity) {
    world->mask[entity] |= POSITION;
    world->position[entity] = (Position) { 0, 0 };
}

// Creates a new sprite sheet for the given entity
void Sprite_New(World *world, int entity,
                int width, int height,
                int frameWidth, int frameHeight, int frames) {
    world->mask[entity] |= SPRITE;
    world->sprite[entity] = (Sprite) {
        width,
        height,
        NULL,
        frameWidth,
        frameHeight,
        frames,
        0,
        NULL
    };
    world->sprite[entity].frames = malloc(frames * sizeof(SDL_Rect));

    // precalculate rects for frames
    for (int i = 0; i < frames; i++) {
        world->sprite[entity].frames[i] = (SDL_Rect) {
            i * frameWidth,
            0,
            frameWidth,
            frameHeight
        };
    }
}

void Sprite_NewFromTexture(World *world, int entity,
                           int width, int height,
                           SDL_Texture *texture,
                           int frameWidth, int frameHeight, int frames) {
    Sprite_New(world, entity, width, height, frameWidth, frameHeight, frames);
    world->sprite[entity].texture = texture;
}

// To David: Why does this take entity as a parameter?
void Text_New(World *world, int entity, char* text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Blended(world->font, text, color);

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
    Sprite_New(world, entity, surface->w, surface->h, surface->w, surface->h, 1);
    world->sprite[entity].texture = texture;

    SDL_FreeSurface(surface);
}

int Position_GetX(World *world, int entity) {
    return world->position[entity].x;
}

void Position_SetX(World *world, int entity, int x) {
    world->position[entity].x = x;
}

void Position_SetY(World *world, int entity, int y) {
    world->position[entity].y = y;
}

void Position_SetXY(World *world, int entity, int x, int y) {
    world->position[entity].x = x;
    world->position[entity].y = y;
}

void World_Free(World *world) {
    TTF_CloseFont(world->font);
    SDL_DestroyRenderer(world->renderer);
    cpSpaceFree(world->space);
    for (int i = 0; i < ENTITY_COUNT; i++) {
        Entity_Free(world, i);
    }
    free(world);
}

void Entity_Free(World *world, int entity) {
    if (entity >= ENTITY_COUNT) return;

    // Free each component of entity. Remember to add when more components
    // get added!
    if (world->mask[entity] & SPRITE)	{
        SDL_DestroyTexture(world->sprite[entity].texture);
        free(world->sprite[entity].frames);
    }
    if (world->mask[entity] & BODY) {
        cpBodyFree(world->body[entity]);
    }

    world->mask[entity] = 0;
}

bool Sprite_HitTest(World *world, int entity, int x, int y) {
    int entityX = world->position[entity].x;
    int entityY = world->position[entity].y;

    int entityWidth = world->sprite[entity].width;
    int entityHeight = world->sprite[entity].height;

    if (x > entityX && x < (entityX + entityWidth) &&
        y > entityY && y < (entityY + entityHeight)) {
        return true;
    }
    return false;
}

void Sprite_NextFrame(World *world, int entity) {
    world->sprite[entity].curFrame++;
    if (world->sprite[entity].curFrame >= world->sprite[entity].numFrames) {
        world->sprite[entity].curFrame = 0;
    }
}
