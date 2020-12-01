#ifndef ECS_COMPONENT_CACHE_H_
#define ECS_COMPONENT_CACHE_H_
#include <cstddef>
#include <unordered_map>

#include "object_pool.h"
#include "entity.h"

namespace ecs {
  template<typename T, std::size_t N=256>
  class component_cache {
    public:

      void insert(ecs::entity_t entity, T const& component) noexcept {
        auto data = component_pool_.create();
        if(data != nullptr) [[likely]] {
          component_lookup_table_[entity] = component_pool_.size();
          entity_lookup_table_[component_pool_.size()] = entity;
          *data = component;
        }
      }

      void remove(ecs::entity_t entity) noexcept {
        auto const component_index = component_lookup_table_[entity];
        auto const swapped_component_indicies = component_pool_.destroy(component_index);
        auto const swapped_component_index = swapped_component_indicies.second;
        auto const swapped_entity = entity_lookup_table_[swapped_component_index];
        component_index[swapped_entity] = swapped_component_indicies.first;
      }

      [[nodiscard]] T& get(ecs::entity_t entity) noexcept {
        static T bogus_component {};
        auto const component_index = component_lookup_table_[entity];
        auto component = component_pool_.get(component_index);
        if(component != nullptr) [[likely]] {
          return *component;
        } 

        return bogus_component;
      }

      [[nodiscard]] inline std::size_t size() const noexcept {
        return component_pool_.size();
      }

      [[nodiscard]] inline T* begin() const noexcept {
        return component_pool_.begin();
      }

      [[nodiscard]] inline T const* const cbegin() const noexcept {
        return component_pool_.cbegin();
      }

      [[nodiscard]] inline T* end() const noexcept {
        return component_pool_.end();
      }

      [[nodiscard]] inline T const* const cend() const noexcept {
        return component_pool_.cend();
      }

    private:
      object_pool<T, N> component_pool_;
      std::unordered_map<ecs::entity_t, std::size_t> component_lookup_table_ {};
      std::unordered_map<std::size_t, ecs::entity_t> entity_lookup_table_ {};
  };
}

#endif

