#ifndef ECS_COMPONENT_POOL_H_
#define ECS_COMPONENT_POOL_H_
#include <cstddef>
#include <unordered_map>

#include "objectpool.h"
#include "entity.h"

namespace ecs {
  template<typename T, std::size_t N=256>
  class component_pool {
    public:
      
      T& add(ecs::entity_t entity) noexcept {
        static T dummy_component {};
        auto const component = data_.create();
        component_lookup_table_[entity] = data_.size();
        entity_lookup_table_[data_.size()] = entity;
        if(component.has_value()) [[likely]] {
          return *component.value();
        } else {
          return dummy_component;
        }
      }

      void insert(ecs::entity_t entity, T const& component) noexcept {
        auto component_data = add(entity);
        component_data = component;
      }

      void remove(ecs::entity_t entity) noexcept {
        auto const component_index = component_lookup_table_[entity];
        auto const swapped_component_indicies = data_.destroy(component_index);
        auto const swapped_component_index = swapped_component_indicies.second;
        auto const swapped_entity = entity_lookup_table_[swapped_component_index];
        component_index[swapped_entity] = swapped_component_indicies.first;
      }

      T& get(ecs::entity_t entity) noexcept {
        auto const component_index = component_lookup_table_[entity];
        return data_.get(component_index).value_or(T{});
      }

      inline std::size_t size() const noexcept {
        return data_.size();
      }

      inline T* begin() const noexcept {
        return data_.begin();
      }

      inline T const* const cbegin() const noexcept {
        return data_.cbegin();
      }

      inline T* end() const noexcept {
        return data_.end();
      }

      inline T const* const cend() const noexcept {
        return data_.cend();
      }

    private:
      object_pool<T, N> data_;
      std::unordered_map<ecs::entity_t, std::size_t> component_lookup_table_ {};
      std::unordered_map<std::size_t, ecs::entity_t> entity_lookup_table_ {};
  };
}

#endif

