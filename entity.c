#include "entity.h"

/* Creates a new entity.
 *
 * Returns: index to the entity, or -1 if no more entities can be created.
 */
int newEntity(World *world) {
  for (int i = 0; i < ENTITY_COUNT; i++) {
    if (world->mask[i] == NONE) {
      return i;
    }
  }
  return -1;
}
