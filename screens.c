#include "defines.h"
#include "screens.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

/*
  TODO: this needs some hardcore refactoring + design work

  - Rendering: we need to handle z-ordering somehow. Also scanning every
    entity on every frame is rather wasteful

  For rendering: have a separate renderer that is given pointers to the
  relevant components
*/
bool Main_Init(Game *game) {
    bool error = false;
    int bg, title, continueGame, quit;
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;

    bg = Entity_New(game->world);
    Position_New(game->world, bg);
    Position_SetXY(game->world, bg, 0, 0);
    surface = IMG_Load("bg.png");
    if (surface == NULL) {
        printf("%d Could not load background image: %s\n", __LINE__, IMG_GetError());
        goto error;
    }
    texture = SDL_CreateTextureFromSurface(game->world->renderer, surface);
    if (texture == NULL) {
        printf("%d Could not load create background texture: %s\n", __LINE__, SDL_GetError());
        goto error;
    }

    Sprite_NewFromTexture(game->world, bg, WINDOW_WIDTH, WINDOW_HEIGHT, texture, surface->w, surface->h, 1, 0);

    title = Entity_New(game->world);
    Position_New(game->world, title);
    Position_SetXY(game->world, title, 720, 151);
    Text_New(game->world, title, "Magical Girl Michael Mauer", (SDL_Color) { 200, 0, 0, 200 });

    continueGame = Entity_New(game->world);
    Position_New(game->world, continueGame);
    Position_SetXY(game->world, continueGame, 720, 250);
    Text_New(game->world, continueGame, "Continue", (SDL_Color) { 0, 0, 0, 200 });

    quit = Entity_New(game->world);
    Position_New(game->world, quit);
    Position_SetXY(game->world, quit, 720, 350);
    Text_New(game->world, quit, "Quit", (SDL_Color) { 0, 0, 0, 200 });

    game->screen->update = Main_Update;
    game->screen->handleEvent = Main_HandleEvent;
    game->screen->render = Main_Render;
    game->screen->end = Main_End;

    game->screen->state = malloc(sizeof(MainState));
    MainState *state = (MainState*) game->screen->state;
    state->bgEntity = bg;
    state->titleEntity = title;
    state->continueEntity = continueGame;
    state->quitEntity = quit;

    goto cleanup;
error:
    error = true;
cleanup:
    SDL_FreeSurface(surface);

    return !error;
}

void Main_Update(Game *game) {
}

void Main_HandleEvent(Game *game, SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (Sprite_HitTest(game->world, ((MainState*) game->screen->state)->continueEntity, x, y)) {
            printf("%s\n", "Clicked continue");
            game->newScreen = Level_Init;
        }
    }
}

void Main_Render(Game *game) {
    for (int i = 0; i < ENTITY_COUNT; i++) {
        if (game->world->mask[i] & SPRITE) {
            int frame = game->world->sprite[i].curFrame;
            Position position = game->world->position[i];
            Sprite sprite = game->world->sprite[i];
            SDL_Rect target = {
                position.x,
                position.y,
                sprite.frameWidth,
                sprite.frameHeight,
            };
            SDL_RenderCopy(game->world->renderer, sprite.texture,
                           &game->world->sprite[i].frames[frame],
                           &target);
        }
    }
}

void Main_End(Game *game) {
    MainState *state = (MainState*) game->screen->state;
    Entity_Free(game->world, state->bgEntity);
    Entity_Free(game->world, state->titleEntity);
    Entity_Free(game->world, state->continueEntity);
    Entity_Free(game->world, state->quitEntity);
    free(game->screen->state);
}

// Based on https://github.com/slembcke/Chipmunk2D/blob/master/Demo/Player.c
// Find the normal vector of the object the player is standing on, if it
// exists. Do so by selecting the most vertical normal of the normals of the
// objects with which the player body is colliding.
static void findGroundNormal(cpBody *body, cpArbiter *arb, cpVect *normal) {
    cpVect n = cpvneg(cpArbiterGetNormal(arb, 0));
    if (n.y > normal->y) {
        *normal = n;
    }
}


static void playerUpdateVelocity(cpBody *body, cpVect g, cpFloat damping, cpFloat dt) {
    Game *game = cpBodyGetUserData(body);
    LevelState *state = (LevelState*) game->screen->state;

    cpBody *playerBody = Physics_GetBody(game->world, state->magicalgirlEntity);
    cpShape *playerShape = Physics_GetShape(game->world, state->magicalgirlEntity);

    cpVect normal = cpvzero;
    cpBodyEachArbiter(playerBody, (cpBodyArbiterIteratorFunc) findGroundNormal, &normal);

    bool grounded = normal.y > 0.0;

    if (grounded) {
        state->playerState &= ~PLAYER_JUMPING;
    }
    else {
        state->playerState |= PLAYER_JUMPING;
    }

    cpBodyUpdateVelocity(body, g, damping, dt);

    if (state->playerState & PLAYER_MOVING) {
        cpFloat vx = (state->playerState & PLAYER_SPRINTING) ?
            PLAYER_SPRINTING_SPEED: PLAYER_SPEED;
        if (!state->facingRight) {
            vx *= -1;
        }

        playerShape->surface_v = cpv(vx, 0.0);
        // set mu, friction coefficient
        playerShape->u = grounded ? 1.0 : 0.0;
    }
    else {
        playerShape->surface_v = cpv(0.0, 0.0);
        playerShape->u = 1.0;
    }
}

bool Level_Init(Game *game) {
    bool error = false;
    SDL_Texture *walkTexture = NULL, *fightTexture = NULL, *hexTexture = NULL;

    game->screen->update = Level_Update;
    game->screen->handleEvent = Level_HandleEvent;
    game->screen->render = Level_Render;
    game->screen->end = Level_End;

    game->screen->state = malloc(sizeof(LevelState));
    LevelState *state = (LevelState*) game->screen->state;
    state->facingRight = false;
    state->playerState = PLAYER_STOPPED;

    walkTexture = Sprite_LoadTexture(game->world, "assets/magicalgirl_walk.png");
    if (walkTexture == NULL) {
        goto error;
    }

    state->magicalgirlEntity = Entity_New(game->world);
    Position_New(game->world, state->magicalgirlEntity);
    Position_SetXY(game->world, state->magicalgirlEntity, 0, 0);
    Sprite_NewFromTexture(game->world, state->magicalgirlEntity,
                          64, 49,
                          walkTexture,
                          64, 49, 19,
                          2);
    SpriteAnimation_New(game->world, state->magicalgirlEntity,
                        0, 1, 9);
    SpriteAnimation_New(game->world, state->magicalgirlEntity,
                        1, 10, 18);
    Sprite_SetFrame(game->world, state->magicalgirlEntity, 1);

    cpFloat mass = 50.0;
    cpFloat width = 24.0 / PIXELS_PER_METER;
    cpFloat height = 49.0 / PIXELS_PER_METER;
    cpFloat moment = cpMomentForBox(mass, width, height);
    cpBody *body = cpBodyNew(mass, moment);
    cpShape *shape = cpBoxShapeNew(body, width, height);
    cpShapeSetCollisionType(shape, PLAYER_COLLISION_TYPE);
    cpShapeSetFriction(shape, 0.3);
    cpBodySetUserData(body, game);
    // use a custom velocity update function
    body->velocity_func = playerUpdateVelocity;
    Physics_New(game->world, state->magicalgirlEntity, body, shape, 20, 0);
    Physics_SetPosition(game->world, state->magicalgirlEntity, 0, 0);

    // TEMPORARY: a floor
    cpShape *floor = cpBoxShapeNew2(game->world->space->staticBody, (cpBB) { -20, -1, 20, 0 });
    cpShapeSetCollisionType(floor, TERRAIN_COLLISION_TYPE);
    cpShapeSetFriction(floor, 1.0);
    Physics_NewStatic(game->world, Entity_New(game->world), floor);

    fightTexture = Sprite_LoadTexture(game->world, "assets/magicalgirl_fight.png");
    if (fightTexture == NULL) {
        goto error;
    }

    state->magicalgirlCombatEntity = Entity_New(game->world);
    Position_New(game->world, state->magicalgirlCombatEntity);
    Position_SetXY(game->world, state->magicalgirlCombatEntity, -64, -64);
    Sprite_NewFromTexture(game->world, state->magicalgirlCombatEntity,
                          192, 192,
                          fightTexture,
                          192, 192, 12,
                          2);
    SpriteAnimation_New(game->world, state->magicalgirlCombatEntity,
                        0, 0, 5);
    SpriteAnimation_New(game->world, state->magicalgirlCombatEntity,
                        1, 6, 11);

    hexTexture = Sprite_LoadTexture(game->world, "assets/hex.png");
    if (hexTexture == NULL) {
        goto error;
    }
    state->hexEntity = Entity_New(game->world);
    Sprite_NewFromTexture(game->world, state->hexEntity,
                          64, 55,
                          hexTexture,
                          64, 55, 1,
                          0);
    Position_New(game->world, state->hexEntity);
    mass = 10.0;
    width = 64.0 / PIXELS_PER_METER;
    height = 55.0 / PIXELS_PER_METER;
    moment = cpMomentForBox(mass, width, height);
    body = cpBodyNew(mass, moment);
    shape = cpBoxShapeNew(body, width, height);
    cpShapeSetCollisionType(shape, TERRAIN_COLLISION_TYPE);
    cpShapeSetFriction(shape, 0.5);
    Physics_New(game->world, state->hexEntity, body, shape, 0, 0);
    Physics_SetPosition(game->world, state->hexEntity, 2, 1);

    goto cleanup;
error:
    error = true;
cleanup:
    return !error;
}

void Level_HandleEvent(Game *game, SDL_Event *event) {
    // TODO: probably not the best way to store state
    static bool leftDown = false;
    static bool rightDown = false;
    LevelState *state = (LevelState*) game->screen->state;
    cpBody *body = Physics_GetBody(game->world, state->magicalgirlEntity);
    bool onGround = !(state->playerState & PLAYER_JUMPING);

    if (event->type == SDL_KEYDOWN) {
        if (!leftDown && event->key.keysym.sym == SDLK_LEFT && onGround) {
            Sprite_SetAnimation(game->world, state->magicalgirlEntity, 0);
            state->playerState &= ~PLAYER_STOPPED;
            state->playerState |= PLAYER_MOVING;
            state->facingRight = false;
            leftDown = true;
        }
        else if (!rightDown && event->key.keysym.sym == SDLK_RIGHT && onGround) {
            Sprite_SetAnimation(game->world, state->magicalgirlEntity, 1);
            state->playerState &= ~PLAYER_STOPPED;
            state->playerState |= PLAYER_MOVING;
            state->facingRight = true;
            rightDown = true;
        }
        else if (event->key.keysym.sym == SDLK_UP && onGround &&
                 (state->playerState & PLAYER_STOPPED || state->playerState & PLAYER_MOVING)) {
            state->playerState |= PLAYER_JUMPING;
            cpBodyApplyImpulse(body, cpv(0, 200), cpv(0, 0));
        }
        else if (event->key.keysym.sym == SDLK_SPACE &&
                 !(state->playerState & PLAYER_ATTACKING)) {
            if (state->facingRight) {
                Sprite_SetAnimation(game->world, state->magicalgirlCombatEntity, 1);
            }
            else {
                Sprite_SetAnimation(game->world, state->magicalgirlCombatEntity, 0);
            }
            state->playerState |= PLAYER_ATTACKING;
        }
        else if (event->key.keysym.sym == SDLK_LSHIFT) {
            state->playerState |= PLAYER_SPRINTING;
        }
    }
    else if (event->type == SDL_KEYUP) {
        if (event->key.keysym.sym == SDLK_LSHIFT) {
            state->playerState &= ~PLAYER_SPRINTING;
        }
        else if (leftDown && event->key.keysym.sym == SDLK_LEFT) {
            if (!rightDown) {
                state->playerState &= ~PLAYER_MOVING;
                state->playerState |= PLAYER_STOPPED;

                Sprite_StopAnimation(game->world, state->magicalgirlEntity);
                Sprite_SetFrame(game->world, state->magicalgirlEntity, 1);
            }
            else {
                state->facingRight = true;

                Sprite_SetAnimation(game->world, state->magicalgirlEntity, 1);
            }
            leftDown = false;
        }
        else if (rightDown && event->key.keysym.sym == SDLK_RIGHT) {
            if (!leftDown) {
                state->playerState &= ~PLAYER_MOVING;
                state->playerState |= PLAYER_STOPPED;

                Sprite_StopAnimation(game->world, state->magicalgirlEntity);
                Sprite_SetFrame(game->world, state->magicalgirlEntity, 10);
            }
            else {
                state->facingRight = false;
                Sprite_SetAnimation(game->world, state->magicalgirlEntity, 0);
            }
            rightDown = false;
        }
    }
}

void Level_Update(Game *game) {
    static int ticksPassed = 0;

    for (int i = 0; i < 10; i++)
        Physics_Step(game->world, 0.004);

    LevelState *state = (LevelState*) game->screen->state;

    cpBody *body = Physics_GetBody(game->world, state->magicalgirlEntity);
    cpVect velocity = cpBodyGetVel(body);
    int animTicks = 4;
    if (state->playerState & PLAYER_SPRINTING) {
        animTicks = 2;
    }

    cpVect pos = cpBodyGetPos(body);
    printf("Stopped: %d Moving: %d Jumping: %d Atk: %d Sprint: %d x: %f y: %f vx: %f vy: %f\n",
           state->playerState & PLAYER_STOPPED,
           state->playerState & PLAYER_MOVING,
           state->playerState & PLAYER_JUMPING,
           state->playerState & PLAYER_ATTACKING,
           state->playerState & PLAYER_SPRINTING,
           pos.x,
           pos.y,
           velocity.x,
           velocity.y);

    Physics_UpdatePosition(game->world, state->magicalgirlEntity);
    Physics_UpdatePosition(game->world, state->hexEntity);

    if (state->playerState & PLAYER_MOVING && !(state->playerState & PLAYER_ATTACKING)) {
        ticksPassed++;
        if (ticksPassed >= animTicks) {
            Sprite_NextFrame(game->world, state->magicalgirlEntity);
            ticksPassed = 0;
        }
    }
    else if (state->playerState & PLAYER_ATTACKING) {
        int x = Position_GetX(game->world, state->magicalgirlEntity);
        int y = Position_GetY(game->world, state->magicalgirlEntity);

        Position_SetXY(game->world, state->magicalgirlCombatEntity, x - 64, y + 64);

        ticksPassed++;
        if (ticksPassed >= 3) {
            Sprite_NextFrame(game->world, state->magicalgirlCombatEntity);
            ticksPassed = 0;
        }

        int curFrame = Sprite_GetFrame(game->world, state->magicalgirlCombatEntity);
        if (curFrame == 5 || curFrame == 11) {
            state->playerState &= ~PLAYER_ATTACKING;
        }
    }
}

void Level_Render(Game *game) {
    LevelState *state = (LevelState*) game->screen->state;
    int entity;
    if (state->playerState & PLAYER_ATTACKING) {
        entity = state->magicalgirlCombatEntity;
    }
    else {
        entity = state->magicalgirlEntity;
    }

    Sprite_Render(game->world, entity);
    Sprite_Render(game->world, state->hexEntity);

    SDL_SetRenderDrawColor(game->world->renderer, 0, 0, 0, 0);
    SDL_RenderDrawLine(game->world->renderer,
                       -20 * PIXELS_PER_METER, WINDOW_HEIGHT - 0 * PIXELS_PER_METER,
                       20 * PIXELS_PER_METER, WINDOW_HEIGHT - 0 * PIXELS_PER_METER);
    SDL_SetRenderDrawColor(game->world->renderer, 0, 191, 255, 255);
}

void Level_End(Game *game) {
    free(game->screen->state);
}
