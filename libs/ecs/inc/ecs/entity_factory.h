#ifndef _ECS_ENTITY_FACTORY_H_
#define _ECS_ENTITY_FACTORY_H_
#include "entity.h"
#include <cstdint>

namespace ecs {
  class entity_factory {
    public:
      inline entity_t generate() noexcept {
        return entity_t{next_valid_id_++};
      }
    private:
      std::uint32_t next_valid_id_ {0};
  };
}
#endif

