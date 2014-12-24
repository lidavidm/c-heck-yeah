# Michael and the Hex World

## Game Design Document

### Goals

### Theme

### Gameplay

### Michael

### Enemies

### Level Design

The player will modify tile properties. (Example: make some tiles bouncy.)

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
- Have some sort of global renderer that tracks z-order/layer,
  etc. Components actually point to this single instance

#### Tilemaps/Backgrounds

- Tyler's custom editor

- Each tile has a collision shape defined for it (in a separate text file
  that lists the points of the bounding polygon)
- Each tile (with a collision shape) is an entity

## Art

Temporary art: http://gaurav.munjal.us/Universal-LPC-Spritesheet-Character-Generator

## Music
