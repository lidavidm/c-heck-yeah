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
    PLAYER_STOPPED = 1 << 0,
    PLAYER_MOVING = 1 << 1,
    PLAYER_JUMPING = 1 << 2,
    PLAYER_ATTACKING = 1 << 3,
    PLAYER_SPRINTING = 1 << 4
} PlayerState;

typedef struct {
    int magicalgirlEntity;
    int magicalgirlCombatEntity;
    int hexEntity;
    PlayerState playerState;
    bool facingRight;
} LevelState;

#define PLAYER_SPEED 1.0
#define PLAYER_SPRINTING_SPEED 1.5

#define PLAYER_COLLISION_TYPE 0
#define ENEMY_COLLISION_TYPE 1
#define TERRAIN_COLLISION_TYPE 2

bool Main_Init(Game *game);
void Main_Update(Game *game);
void Main_HandleEvent(Game *game, SDL_Event *event);
void Main_Render(Game *game);
void Main_End(Game *game);

bool Level_Init(Game *game);
void Level_Update(Game *game);
void Level_HandleEvent(Game *game, SDL_Event *event);
int Level_HandleTerrainCollision(cpArbiter *arb, struct cpSpace *space, void *data);
void Level_SetPlayerVelocity(LevelState *state, cpBody *body);
void Level_Render(Game *game);
void Level_End(Game *game);

#endif
