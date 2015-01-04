#include "defines.h"
#include "screens.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

// Attempts to load the background for the title screen. Returns the entity
// number for the background, or -1 if surface fails, or -2 if texture fails to load
static int initBG(Game *game)  {
    int bg;

    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    
    bg = Entity_New(game->world);
    Position_New(game->world, bg);
    Position_SetXY(game->world, bg, 0, 0);
     
    surface = IMG_Load("bg.png");
    texture = SDL_CreateTextureFromSurface(game->world->renderer, surface);
    if (surface == NULL) return -1;
    if (texture == NULL)    {
        SDL_FreeSurface(surface);    
        return -2;
    }
    Sprite_NewFromTexture(game->world, bg, WINDOW_WIDTH, WINDOW_HEIGHT, texture,
        surface->w, surface->h, 1, 0);
    
    SDL_FreeSurface(surface);
    return bg; // all is well
}
/*
  TODO: this needs some hardcore refactoring + design work

  - Each screen needs some place to store its state. May need to pass a void
    pointer around

  - Rendering: we need to handle z-ordering somehow. Also scanning every
    entity on every frame is rather wasteful

  For rendering: have a separate renderer that is given pointers to the
  relevant components
*/
bool Main_Init(Game *game) {
    bool error = false;
    int bg, title, continueGame, quit;
    
    switch (bg = initBG(game))   {
        case -1:
            printf("%d Could not load background image: %s\n", __LINE__, IMG_GetError());
            goto error;
        case -2:
            printf("%d Could not load create background texture: %s\n", __LINE__, SDL_GetError());
            goto error;
    }

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

bool Level_Init(Game *game) {
    bool error = false;
    SDL_Surface *magicalgirlSurface;
    SDL_Texture *magicalgirlTexture = NULL;

    game->screen->update = Level_Update;
    game->screen->handleEvent = Level_HandleEvent;
    game->screen->render = Level_Render;
    game->screen->end = Level_End;

    game->screen->state = malloc(sizeof(LevelState));
    LevelState *state = (LevelState*) game->screen->state;
    state->velocity = 0;
    state->playerState = PLAYER_STOPPED;

    magicalgirlSurface = IMG_Load("assets/magicalgirl_walk.png");
    if (magicalgirlSurface == NULL) {
        printf("%d Could not load background image: %s\n", __LINE__, IMG_GetError());
        goto error;
    }
    magicalgirlTexture = SDL_CreateTextureFromSurface(game->world->renderer, magicalgirlSurface);
    if (magicalgirlTexture == NULL) {
        printf("%d Could not load create background texture: %s\n", __LINE__, SDL_GetError());
        goto error;
    }

    state->magicalgirlEntity = Entity_New(game->world);
    Position_New(game->world, state->magicalgirlEntity);
    Position_SetXY(game->world, state->magicalgirlEntity, 0, 0);
    Sprite_NewFromTexture(game->world, state->magicalgirlEntity,
                          64, 64,
                          magicalgirlTexture,
                          64, 64, 19,
                          2);
    SpriteAnimation_New(game->world, state->magicalgirlEntity,
                        0, 1, 9);
    SpriteAnimation_New(game->world, state->magicalgirlEntity,
                        1, 10, 18);
    Sprite_SetFrame(game->world, state->magicalgirlEntity, 1);

    cpFloat mass = 50.0;
    cpFloat width = 64.0 / PIXELS_PER_METER;
    cpFloat height = 64.0 / PIXELS_PER_METER;
    cpFloat moment = cpMomentForBox(50.0, width, height);
    cpBody *body = cpBodyNew(mass, moment);
    cpShape *shape = cpBoxShapeNew(body, width, height);
    Physics_New(game->world, state->magicalgirlEntity, body, shape);
    Physics_SetPosition(game->world, state->magicalgirlEntity, 0, 0);

    // TEMPORARY: a floor
    Physics_NewStatic(game->world, Entity_New(game->world),
                      cpSegmentShapeNew(game->world->space->staticBody, cpv(-20, -11), cpv(20, -11), 1));

    magicalgirlSurface = IMG_Load("assets/magicalgirl_fight.png");
    if (magicalgirlSurface == NULL) {
        printf("%d Could not load background image: %s\n", __LINE__, IMG_GetError());
        goto error;
    }
    magicalgirlTexture = SDL_CreateTextureFromSurface(game->world->renderer, magicalgirlSurface);
    if (magicalgirlTexture == NULL) {
        printf("%d Could not load create background texture: %s\n", __LINE__, SDL_GetError());
        goto error;
    }

    state->magicalgirlCombatEntity = Entity_New(game->world);
    Position_New(game->world, state->magicalgirlCombatEntity);
    Position_SetXY(game->world, state->magicalgirlCombatEntity, -64, -64);
    Sprite_NewFromTexture(game->world, state->magicalgirlCombatEntity,
                          192, 192,
                          magicalgirlTexture,
                          192, 192, 12,
                          2);
    SpriteAnimation_New(game->world, state->magicalgirlCombatEntity,
                        0, 0, 5);
    SpriteAnimation_New(game->world, state->magicalgirlCombatEntity,
                        1, 6, 11);

    SDL_SetRenderDrawColor(game->world->renderer, 0, 191, 255, 255);

    goto cleanup;

error:
    error = true;

cleanup:
    return !error;
}

void Level_HandleEvent(Game *game, SDL_Event *event) {
    LevelState *state = (LevelState*) game->screen->state;
    cpBody *body = Physics_GetBody(game->world, state->magicalgirlEntity);
    cpVect velocity = cpBodyGetVel(body);
    bool onGround = velocity.y > -0.1 && velocity.y < 0.1;

    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_LEFT && onGround &&
            (state->playerState == PLAYER_STOPPED || velocity.x >= 0)) {
            Sprite_SetAnimation(game->world, state->magicalgirlEntity, 0);
            state->playerState = PLAYER_MOVING;
            state->velocity = -PLAYER_SPEED;
            cpBodySetVel(body, cpv(-PLAYER_SPEED, velocity.y));
        }
        else if (event->key.keysym.sym == SDLK_RIGHT && onGround &&
                 (state->playerState == PLAYER_STOPPED || velocity.x <= 0)) {
            Sprite_SetAnimation(game->world, state->magicalgirlEntity, 1);
            state->playerState = PLAYER_MOVING;
            state->velocity = PLAYER_SPEED;
            cpBodySetVel(body, cpv(PLAYER_SPEED, velocity.y));
        }
        else if (event->key.keysym.sym == SDLK_UP && onGround &&
                 (state->playerState == PLAYER_STOPPED || state->playerState == PLAYER_MOVING)) {
            // Sprite_SetAnimation(game->world, state->magicalgirlEntity, 1);
            // TODO: state->playerState |= PLAYER_JUMPING;
            cpBodyApplyImpulse(body, cpv(0, 200), cpv(0, 0));
        }
        else if (event->key.keysym.sym == SDLK_SPACE &&
                 (state->playerState == PLAYER_STOPPED ||
                  state->playerState == PLAYER_MOVING)) {
            if (state->velocity < 0 ||
                Sprite_GetFrame(game->world, state->magicalgirlEntity) == 1) {
                // facing left
                Sprite_SetAnimation(game->world, state->magicalgirlCombatEntity, 0);
            }
            else {
                // facing right
                Sprite_SetAnimation(game->world, state->magicalgirlCombatEntity, 1);
            }
            cpBodySetVel(body, cpv(0, velocity.y));
            state->playerState = PLAYER_ATTACKING;
            int x = Position_GetX(game->world, state->magicalgirlEntity);
            int y = Position_GetY(game->world, state->magicalgirlEntity);

            Position_SetXY(game->world, state->magicalgirlCombatEntity, x - 64, y - 64);
        }
    }
    else if (event->type == SDL_KEYUP) {
        if ((event->key.keysym.sym == SDLK_LEFT && state->velocity < 0) ||
            (event->key.keysym.sym == SDLK_RIGHT && state->velocity > 0)) {
            state->playerState = PLAYER_STOPPED;
            state->velocity = 0;
            cpBodySetVel(body, cpv(0, velocity.y));
            Sprite_StopAnimation(game->world, state->magicalgirlEntity);

            if (event->key.keysym.sym == SDLK_LEFT) {
                Sprite_SetFrame(game->world, state->magicalgirlEntity, 1);
            }
            else {
                Sprite_SetFrame(game->world, state->magicalgirlEntity, 10);
            }
        }
    }
}

void Level_Update(Game *game) {
    static int ticksPassed = 0;

    Physics_Step(game->world, 0.04);

    LevelState *state = (LevelState*) game->screen->state;
    Physics_UpdatePosition(game->world, state->magicalgirlEntity);
    if (state->playerState == PLAYER_MOVING) {
        ticksPassed++;
        if (ticksPassed >= 4) {
            Sprite_NextFrame(game->world, state->magicalgirlEntity);
            ticksPassed = 0;
        }
    }
    else if (state->playerState == PLAYER_ATTACKING) {
        int x = Position_GetX(game->world, state->magicalgirlEntity);
        int y = Position_GetY(game->world, state->magicalgirlEntity);

        Position_SetXY(game->world, state->magicalgirlCombatEntity, x - 64, y - 64);

        ticksPassed++;
        if (ticksPassed >= 3) {
            Sprite_NextFrame(game->world, state->magicalgirlCombatEntity);
            ticksPassed = 0;
        }

        int curFrame = Sprite_GetFrame(game->world, state->magicalgirlCombatEntity);
        if (curFrame == 5 || curFrame == 11) {
            if (state->velocity == 0) {
                state->playerState = PLAYER_STOPPED;
            }
            else {
                state->playerState = PLAYER_MOVING;
                cpBody *body = Physics_GetBody(game->world, state->magicalgirlEntity);
                cpVect velocity = cpBodyGetVel(body);
                cpBodySetVel(body, cpv(state->velocity, velocity.y));
            }
        }
    }
}

void Level_Render(Game *game) {
    LevelState *state = (LevelState*) game->screen->state;
    int entity;
    if (state->playerState == PLAYER_ATTACKING) {
        entity = state->magicalgirlCombatEntity;
    }
    else {
        entity = state->magicalgirlEntity;
    }

    int frame = game->world->sprite[entity].curFrame;
    Position position = game->world->position[entity];
    Sprite sprite = game->world->sprite[entity];
    SDL_Rect target = {
        position.x,
        position.y,
        sprite.frameWidth,
        sprite.frameHeight,
    };

    SDL_RenderCopy(game->world->renderer, sprite.texture,
                   &game->world->sprite[entity].frames[frame],
                   &target);
}

void Level_End(Game *game) {
    free(game->screen->state);
}


bool Editor_Main_Init(Game *game)   {
    bool error = false;
    int bg, title, newMap, loadMap, quit;
            
    switch (bg = initBG(game))  {
        case -1:
            printf("%d Could not load background image: %s\n", __LINE__, IMG_GetError());
            goto error;
        case -2:
            printf("%d Could not load create background texture: %s\n", __LINE__, SDL_GetError());
            goto error;
    }

    title = Entity_New(game->world);
    Position_New(game->world, title);
    Position_SetXY(game->world, title, 600, 50);
    Text_New(game->world, title, "Magical Michael's Magical Editor", (SDL_Color) { 200, 0, 0, 200});

    newMap = Entity_New(game->world);
    Position_New(game->world, newMap);
    Position_SetXY(game->world, newMap, 700, 250);
    Text_New(game->world, newMap, "New Map", (SDL_Color) { 0, 0, 0, 200});

    loadMap = Entity_New(game->world);
    Position_New(game->world, loadMap);
    Position_SetXY(game->world, loadMap, 1000, 250);
    Text_New(game->world, loadMap, "Load Map", (SDL_Color) { 0, 0, 0, 200});

    quit = Entity_New(game->world);
    Position_New(game->world, quit);
    Position_SetXY(game->world, quit, 720, 350);
    Text_New(game->world, quit, "Quit", (SDL_Color) { 0, 0, 0, 200 });

    game->screen->update = Main_Update;
    game->screen->handleEvent = Editor_Main_HandleEvent;
    game->screen->render = Main_Render;
    game->screen->end = Editor_Main_End;

    game->screen->state = malloc(sizeof(EditorMainState));
    EditorMainState *state = (EditorMainState*) game->screen->state;

    state->bgEntity = bg;
    state->titleEntity = title;
    state->loadMapEntity = loadMap;
    state->newMapEntity = newMap;
    state->quitEntity = quit;

    goto cleanup;

error:
    error = true;
    
cleanup:
    return !error;

}

void Editor_Main_HandleEvent(Game *game, SDL_Event *event)  {
    //TODO
}
void Editor_Main_End(Game *game)    {
    //TODO
}

