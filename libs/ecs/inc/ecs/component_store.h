#ifndef ECS_INC_ECS_COMPONENT_STORE_H_
#define ECS_INC_ECS_COMPONENT_STORE_H_
#include "containers/array.h"
#include "types.h"
#include <unordered_map>
#include <typeindex>

namespace acorn {
namespace ecs {
class component_store {
  public:
    
    template<typename T>
    void insert_component(entity_t entity, T& component) {
      auto const component_index = std::type_index(typeid(T));
      auto const entity_index = static_cast<std::size_t>(entity);
      if(!component_data_.contains(component_index)) {
        component_data_[component_index] = reinterpret_cast<void*>(new acorn::ecs::containers::array<T>());
      }

      (reinterpret_cast<acorn::ecs::containers::array<T>*>(component_data_[component_index]))->get(entity_index) = component;
    }

    template<typename T>
    T& get_component(entity_t entity) {
      auto const component_index = std::type_index(typeid(T));
      auto const entity_index = static_cast<std::size_t>(entity);
      if(!component_data_.contains(component_index)) {
        component_data_[component_index] = reinterpret_cast<void*>(new acorn::ecs::containers::array<T>());
      }

      return (reinterpret_cast<acorn::ecs::containers::array<T>*>(component_data_[component_index]))->get(entity_index);
    }
  private:
    std::unordered_map<std::type_index, void*> component_data_{};
};
}
}
#endif

