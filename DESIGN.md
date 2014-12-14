# Michael and the Hex World

## Game Design Document

### Goals

### Theme

### Gameplay

### Michael

### Enemies

### Level Design

### Victory

## Code Structure

How do we design things without classes??

How do we represent game states?

What game states do we need?

### Entities

Entity-component system?

http://www.gamedev.net/page/resources/_/technical/game-programming/implementing-component-entity-systems-r3382

- What kinds of entities do we need?

### Update Loop

http://gameprogrammingpatterns.com/game-loop.html#play-catch-up

- Needs to be kept frame-independent
- Clamp to 60FPS (or lower, if on battery)

### Physics

http://gamedev.stackexchange.com/questions/2799/good-2d-platformer-physics

https://chipmunk-physics.net/release/ChipmunkLatest-Docs/

### Rendering

- Animated sprites (spritesheets)
- Each entity is occlusion culled and then rendered
  - Possible to use physics engine to do this culling?

#### Tilemaps/Backgrounds

- Tiled Map Editor (http://mapeditor.org)
- https://github.com/baylej/tmx/blob/master/examples/sdl.c
