#ifndef ECS_ENTITY_H_
#define ECS_ENTITY_H_
#include <cstdint>

namespace ecs {
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
}
#endif

