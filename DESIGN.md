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

### Entities

Entity-component system?

http://www.gamedev.net/page/resources/_/technical/game-programming/implementing-component-entity-systems-r3382

- What kinds of entities do we need?

### Update Loop

http://gameprogrammingpatterns.com/game-loop.html#play-catch-up

- Needs to be kept frame-independent
- Clamp to 60FPS (or lower, if on battery)

### Physics

### Rendering

- Animated sprites (spritesheets)
- Each entity is occlusion culled and then rendered
  - Possible to use physics engine to do this culling?
