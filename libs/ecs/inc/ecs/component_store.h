#ifndef ECS_INC_ECS_COMPONENT_STORE_H_
#define ECS_INC_ECS_COMPONENT_STORE_H_
#include "containers/array.h"
#include "types.h"
#include <unordered_map>
#include <unordered_set>
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
      
      component_metadata_[component_index].emplace(entity);

      (reinterpret_cast<acorn::ecs::containers::array<T>*>(component_data_[component_index]))->get(entity_index) = component;
    }

    template<typename T>
    void remove_component(entity_t entity) {
      auto const component_index = std::type_index(typeid(T));
      if(component_metadata_.find(component_index) != std::end(component_metadata_)) {
        component_metadata_[component_index].erase(entity);
      }
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

    template<typename T>
    bool has_component(entity_t entity) {
      auto const component_index = std::type_index(typeid(T));
      if(component_metadata_.find(component_index) != std::end(component_metadata_)) {
        return component_metadata_[component_index].contains(entity);
      } 
      return false;
    }
  private:
    std::unordered_map<std::type_index, void*> component_data_{};
    std::unordered_map<std::type_index, std::unordered_set<entity_t>> component_metadata_{};
};
}
}
#endif

