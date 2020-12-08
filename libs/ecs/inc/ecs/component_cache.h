#ifndef ECS_COMPONENT_CACHE_H_
#define ECS_COMPONENT_CACHE_H_
#include <cstdint>
#include <memory>

#include "entity.h"

namespace ecs {
  template<typename T, std::uint32_t N=256>
  class component_cache {
    using component_data = std::unique_ptr<T, void(*)(T* data)>;
    public:

      [[nodiscard]] T& operator[](entity_t entity) const noexcept {
        auto const index = static_cast<std::size_t>(get_index(entity));
        return data_.get()[index];
      }

      [[nodiscard]] inline std::size_t size() const noexcept {
        return N;
      }

    private:
      component_data data_{new T[N], [](T* data){ delete[] data; }};
 };
}

#endif

