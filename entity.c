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
                int frameWidth, int frameHeight, int frames,
                int animations) {
    world->mask[entity] |= SPRITE;
    world->sprite[entity] = (Sprite) {
        width,
        height,
        NULL,
        frameWidth,
        frameHeight,
        frames,
        0,
        NULL,
        animations,
        0,
        NULL
    };
    world->sprite[entity].frames = malloc(frames * sizeof(SDL_Rect));
    world->sprite[entity].animations = malloc(animations * sizeof(SpriteAnimation));

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
                           int frameWidth, int frameHeight, int frames,
                           int animations) {
    Sprite_New(world, entity, width, height, frameWidth, frameHeight, frames, animations);
    world->sprite[entity].texture = texture;
}

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
    Sprite_New(world, entity, surface->w, surface->h, surface->w, surface->h, 1, 0);
    world->sprite[entity].texture = texture;

    SDL_FreeSurface(surface);
}

int Position_GetY(World *world, int entity) {
    return world->position[entity].y;
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
  for (int i = 0; i < ENTITY_COUNT; i++) {
  	Entity_Free(world, i);
  }
  cpSpaceFree(world->space);
  free(world);
}

void Entity_Free(World *world, int entity) {
    if (entity >= ENTITY_COUNT) return;

    // Free each component of entity. Remember to add when more components
    // get added!
    if (world->mask[entity] & SPRITE)	{
        SDL_DestroyTexture(world->sprite[entity].texture);
        free(world->sprite[entity].frames);
        /* for (int i = 0; i < world->sprite[entity].numAnimations; i++) { */
        /*     free(world->sprite[entity].animations[i].frames); */
        /* } */
        free(world->sprite[entity].animations);
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

int Sprite_GetFrame(World *world, int entity) {
    Sprite *sprite = &world->sprite[entity];
    return sprite->curFrame;
}

void Sprite_SetFrame(World *world, int entity, int frame) {
    Sprite *sprite = &world->sprite[entity];
    if (frame >= sprite->numFrames || frame < 0) {
        printf("%d Invalid frame index %d\n", __LINE__, frame);
        // TODO: handle error
        return;
    }
    // TODO: check that frame belongs to current animation
    sprite->curFrame = frame;
}

void Sprite_SetAnimation(World *world, int entity, int animation) {
    Sprite *sprite = &world->sprite[entity];
    if (animation >= sprite->numAnimations || animation < 0) {
        printf("%d Invalid animation index %d\n", __LINE__, animation);
        // TODO: handle error
        return;
    }
    sprite->curAnimation = animation;
    sprite->curFrame = sprite->animations[animation].start;
}

void Sprite_StopAnimation(World *world, int entity) {
    Sprite *sprite = &world->sprite[entity];
    sprite->curAnimation = SPRITE_ANIMATION_NONE;
}

void SpriteAnimation_New(World *world, int entity,
                         int animation, int start, int end) {
    Sprite *sprite = &world->sprite[entity];
    if (animation >= sprite->numAnimations || animation < 0) {
        printf("%d Invalid animation index %d\n", __LINE__, animation);
        // TODO: handle error
        return;
    }
    if (start < 0 || end > sprite->numFrames || start > end) {
        printf("%d Invalid animation bounds %d %d\n", __LINE__, start, end);
    }
    sprite->animations[animation] = (SpriteAnimation) {
        start, end
    };
}

void Sprite_NextFrame(World *world, int entity) {
    Sprite *sprite = &world->sprite[entity];
    if (sprite->curAnimation == SPRITE_ANIMATION_NONE) return;

    SpriteAnimation *anim = &sprite->animations[sprite->curAnimation];
    sprite->curFrame++;
    if (sprite->curFrame > anim->end) {
        sprite->curFrame = anim->start;
    }
}
