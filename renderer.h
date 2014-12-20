#ifndef RENDERER
#define RENDERER

typedef struct Renderer {
} Renderer;

void addSprite(Renderer *renderer, Sprite *sprite, int layer);
void render(World *world, Renderer *renderer);

#endif
