#ifndef SCREENS
#define SCREENS

#include <SDL.h>

#include "entity.h"

struct Game;

// http://stackoverflow.com/questions/2438539/
// Function pointers shouldn't incur much (if any) overhead, while switch
// statements do
typedef struct GameScreen {
    void (*update)(struct Game *game);
    void (*handleEvent)(struct Game *game, SDL_Event *event);
    void (*render)(struct Game *game);
    void (*end)(struct Game *game);
    void *state;
} GameScreen;

typedef struct Game {
    World *world;
    GameScreen *screen;
    bool (*newScreen)(struct Game *game); // non-NULL = change game screen
} Game;

typedef struct {
    int bgEntity;
    int titleEntity;
    int continueEntity;
    int quitEntity;
} MainState;

typedef enum PlayerState {
    PLAYER_STOPPED,
    PLAYER_MOVING,
    PLAYER_ATTACKING
} PlayerState;

typedef struct {
    int magicalgirlEntity;
    int magicalgirlCombatEntity;
    PlayerState playerState;
    int velocity;
} LevelState;

bool Main_Init(Game *game);
void Main_Update(Game *game);
void Main_HandleEvent(Game *game, SDL_Event *event);
void Main_Render(Game *game);
void Main_End(Game *game);

bool Level_Init(Game *game);
void Level_Update(Game *game);
void Level_HandleEvent(Game *game, SDL_Event *event);
void Level_Render(Game *game);
void Level_End(Game *game);

#endif
