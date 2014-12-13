# Michael and the Hex World

## Rendering

### Entity Struct

- Frame-independent update() loop
- Entity struct, each with update method (or, some subsystems are called and
  responsible for updating each entity)
- Each entity is occlusion culled and then rendered
- Chipmunk Physics?
