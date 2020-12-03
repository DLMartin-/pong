#ifndef ECS_COMPONENT_CACHE_H_
#define ECS_COMPONENT_CACHE_H_
#include <cstdint>
#include <unordered_map>

#include "entity.h"
#include "object_pool.h"

namespace ecs {
  template<typename T, std::uint32_t N=256>
  class component_cache {
    public:

      void insert(entity_t entity, T const& component) noexcept {
        components_.create() = component;
        component_index_lookup_.insert_or_assign(entity, components_.size() - 1);
        entity_index_lookup_.insert_or_assign(components_.size() - 1, entity);
      }

      void remove(entity_t entity) noexcept {
        auto const index = component_index_lookup_.at(entity);
        auto const [destroyed_index, swapped_index] = components_.destroy(index);
        auto const swapped_entity = entity_index_lookup_[swapped_index];
        component_index_lookup_[swapped_entity] = destroyed_index;
        entity_index_lookup_[destroyed_index] = swapped_entity;
      }

      [[nodiscard]] inline T& get(entity_t entity) const noexcept {
        auto const index = component_index_lookup_.at(entity);
        return components_.get(index);
      }

      [[nodiscard]] inline std::size_t size() const noexcept {
        return components_.size();
      }

      [[nodiscard]] inline T* begin() const noexcept {
        return components_.begin();
      }

      [[nodiscard]] inline T const* const cbegin() const noexcept {
        return components_.cbegin();
      }

      [[nodiscard]] inline T* end() const noexcept {
        return components_.end();
      }

      [[nodiscard]] inline T const* const cend() const noexcept {
        return components_.cend();
      }
    
    private:
      object_pool<T, N> components_{};
      std::unordered_map<entity_t, std::size_t> component_index_lookup_{};
      std::unordered_map<std::size_t, entity_t> entity_index_lookup_{};
 };
}

#endif

