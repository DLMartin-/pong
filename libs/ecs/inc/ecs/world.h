#ifndef ECS_OBJECT_POOL_H_
#define ECS_OBJECT_POOL_H_
#include "entity.h"
#include "entity_factory.h"

namespace ecs {
  class world {
    friend class entity;
    public:

      [[nodiscard]] entity create_entity();
      void destroy_entity(ecs::entity entity);
    private:
      ecs::entity_factory factory_ {};
  };
}
#endif

