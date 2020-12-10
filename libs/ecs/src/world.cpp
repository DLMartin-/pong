#include "world.h"

ecs::entity ecs::world::create_entity() {
  return ecs::entity{this};
}

void ecs::world::destroy_entity(ecs::entity entity) {

}
