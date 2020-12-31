#ifndef ECS_INC_ECS_ENTITY_POOL_H_
#define ECS_INC_ECS_ENTITY_POOL_H_
#include "types.h"
#include "containers/array.h"

#include <optional>
#include <algorithm>
#include <iterator>

namespace acorn {
namespace ecs {
class entity_pool {
public:
  entity_pool() {
    auto populate_pool_data = [this]{
      std::for_each(std::begin(pool_), std::end(pool_), [i = 0](auto& val) mutable {
        val = acorn::ecs::entity_t(i++);
      });
    };

    populate_pool_data();
  }

  [[nodiscard]] entity_t create_entity() {
    return pool_[end_++];
  }

  void destroy_entity(entity_t entity) {
    if(auto swap_idx = find_index(entity); swap_idx.has_value()) {
      std::swap(pool_[swap_idx.value()], pool_[--end_]);
    }
  }

private:
  std::optional<std::size_t> find_index(entity_t entity) {
    for(auto i = 0; i < end_; ++i) {
      if(pool_[i] == entity) {
        return std::optional{i};
      }
    }

    return std::nullopt;
  }

  acorn::ecs::containers::array<entity_t> pool_{};  
  std::size_t end_{0};
};
}
}
#endif

