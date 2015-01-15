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
    void *state; // We should probably use a union for this, no? I've defined
                 // one below. I'm not sure what code depends on this being a
                 // void pointer though so i didn't change one
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

// Game state the level editor title screen
typedef struct {
    int bgEntity;
    int titleEntity;
    int newMapEntity;
    int loadMapEntity;
    int quitEntity;
} EditorMainState;

// Game state when the level editor is in editting state 
typedef struct  {
    int menuEntity;
    int selectedTileEntity;
} EditorLevelState;

// We could use this to replace void pointers for state? Much cleaner solution
typedef union {
    MainState main;
    LevelState level;
    EditorMainState editorMain;
    EditorLevelState editorLevel;
} State;

#define PLAYER_SPEED 1.0

bool Main_Init(Game *game);
void Main_Update(Game *game);
void Main_HandleEvent(Game *game, SDL_Event *event);
void Main_Render(Game *game);
void Main_End(Game *game);

// Functions for the editor title screen. Render and update currently use regular (non-editor)
                                       // Functions instead.
bool Editor_Main_Init(Game *game);
void Editor_Main_HandleEvent(Game *game, SDL_Event *event);
void Editor_Main_End(Game *game);

bool Level_Init(Game *game);
void Level_Update(Game *game);
void Level_HandleEvent(Game *game, SDL_Event *event);
void Level_Render(Game *game);
void Level_End(Game *game);

// Functions for the actual 
bool Editor_Level_Init(Game *game);
void Editor_Level_Update(Game *game);
void Editor_Level_HandleEvent(Game *game, SDL_Event *event);
void Editor_Level_Render(Game *game);
void Editor_Level_End(Game *game);
#endif
