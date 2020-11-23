#include "pool.h"

#include <algorithm>

ecs::pool::pool() : data_(new std::byte[1024], [](auto data){delete[] data;}), 
                    end_(1024), 
                    pool_end_(0) {
}

std::byte& ecs::pool::create() noexcept {
  return data_.get()[pool_end_++];
}

std::pair<std::uint32_t, std::uint32_t> ecs::pool::destroy(std::uint32_t index) noexcept {
  std::swap(data_.get()[index], data_.get()[--pool_end_]);
  return {index, pool_end_};
}

std::byte& ecs::pool::get(std::uint32_t index) const noexcept {
  return data_.get()[index];
}
