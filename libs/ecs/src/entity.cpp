#include "entity.h"
#include "world.h"

ecs::entity::entity(ecs::world* world) : world_{world} {
  guid_ = world_->factory_.generate();
}

void ecs::entity::attach_component() {

}

void ecs::entity::remove_component() {

}

