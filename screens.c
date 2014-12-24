#include "defines.h"
#include "screens.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

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

bool Level_Init(Game *game) {
    bool error = false;
    SDL_Surface *magicalgirlSurface;
    SDL_Texture *magicalgirlTexture = NULL;

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

    game->screen->update = Level_Update;
    game->screen->handleEvent = Level_HandleEvent;
    game->screen->render = Level_Render;
    game->screen->end = Level_End;

    game->screen->state = malloc(sizeof(LevelState));
    LevelState *state = (LevelState*) game->screen->state;

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

    SDL_SetRenderDrawColor(game->world->renderer, 0, 191, 255, 255);

    goto cleanup;
error:
    error = true;
cleanup:
    SDL_FreeSurface(magicalgirlSurface);
    return !error;
}

void Level_HandleEvent(Game *game, SDL_Event *event) {
    LevelState *state = (LevelState*) game->screen->state;

    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_LEFT && (!state->moving || state->velocity > 0)) {
            Sprite_SetAnimation(game->world, state->magicalgirlEntity, 0);
            state->moving = true;
            state->velocity = -2;
        }
        else if (event->key.keysym.sym == SDLK_RIGHT && (!state->moving || state->velocity < 0)) {
            Sprite_SetAnimation(game->world, state->magicalgirlEntity, 1);
            state->moving = true;
            state->velocity = 2;
        }
    }
    else if (event->type == SDL_KEYUP) {
        if ((event->key.keysym.sym == SDLK_LEFT && state->velocity < 0) ||
            (event->key.keysym.sym == SDLK_RIGHT && state->velocity > 0)) {
            state->moving = false;
            state->velocity = 0;
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

    LevelState *state = (LevelState*) game->screen->state;
    if (state->moving) {
        ticksPassed++;

        if (ticksPassed >= 4) {
            Sprite_NextFrame(game->world, state->magicalgirlEntity);
            ticksPassed = 0;
        }

        Position_SetX(game->world, state->magicalgirlEntity,
                      Position_GetX(game->world, state->magicalgirlEntity) + state->velocity);
    }
}

void Level_Render(Game *game) {
    int entity = ((LevelState*) game->screen->state)->magicalgirlEntity;
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
