#ifndef ECS_ENTITY_H_
#define ECS_ENTITY_H_
#include <cstdint>

namespace ecs {
  enum struct entity_t : std::uint32_t {};
  enum struct entity_index : std::uint32_t {};
  enum struct entity_version : std::uint8_t {};

  constexpr std::uint32_t entity_index_bitmask = 0x00ff'ffff;
  constexpr std::uint32_t entity_version_bitmask = 0x7f00'0000;
  constexpr std::uint32_t entity_invalid_bitmask = 0x8000'0000;

  inline entity_index get_index(entity_t entity) noexcept {
    return entity_index(static_cast<std::uint32_t>(entity) & entity_index_bitmask);
  } 

  inline entity_version get_version(entity_t entity) noexcept {
    return entity_version(static_cast<std::uint32_t>(entity) & entity_version_bitmask);
  }

  inline bool is_valid(entity_t entity) noexcept {
    return (static_cast<std::uint32_t>(entity) & entity_invalid_bitmask) != entity_invalid_bitmask;
  }
}
#endif

