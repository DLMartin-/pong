#ifndef _ECS_ENTITY_FACTORY_H_
#define _ECS_ENTITY_FACTORY_H_
#include "entity.h"
#include <cstdint>

namespace ecs {

  template<std::uint16_t N = 256>
  class entity_factory {
    public:
      entity_t generate() noexcept {
        static constexpr entity_t dummy_entity {entity_invalid_bitmask};
        
        if(next_valid_id_ < N) [[likely]] {
          return entity_t{next_valid_id_++};
        }

        return dummy_entity;
      }
    private:
      std::uint32_t next_valid_id_ {0};
  };
}
#endif

