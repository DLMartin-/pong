#ifndef ECS_ENTITY_H_
#define ECS_ENTITY_H_
#include <cstdint>

namespace ecs {
  class world;
  enum struct entity_t : std::uint32_t {};
  enum struct entity_index : std::uint32_t {};
  enum struct entity_version : std::uint8_t {};

  constexpr std::uint32_t entity_index_bitmask = 0x00ff'ffff;
  constexpr std::uint32_t entity_version_bitmask = 0xff00'0000;

  inline entity_index get_index(entity_t entity) noexcept {
    return entity_index(static_cast<std::uint32_t>(entity) & entity_index_bitmask);
  } 

  inline entity_version get_version(entity_t entity) noexcept {
    return entity_version(static_cast<std::uint32_t>(entity) & entity_version_bitmask);
  }

  class entity {
    friend class world;
    public:
      void attach_component();
      void remove_component();
    private:
      explicit entity(world* world);

      world* world_{nullptr};
      entity_t guid_{0x0};
  };
}
#endif

