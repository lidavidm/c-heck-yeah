# Michael and the Hex World

## Code Structure

How do we design things without classes??

### Entities

Entity-component system?

http://www.gamedev.net/page/resources/_/technical/game-programming/implementing-component-entity-systems-r3382

### Update Loop

http://gameprogrammingpatterns.com/game-loop.html#play-catch-up

- Needs to be kept frame-independent
- Clamp to 60FPS (or lower, if on battery)

### Physics

### Rendering

- Frame-independent update() loop
- Entity struct, each with update method (or, some subsystems are called and
  responsible for updating each entity)
- Each entity is occlusion culled and then rendered
- Chipmunk Physics?
