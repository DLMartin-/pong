#ifndef ECS_POOL_H_
#define ECS_POOL_H_
#include <cstddef>
#include <memory>

namespace ecs {

  class pool {
    public:
      pool();

      std::byte& create() noexcept;
      std::pair<std::uint32_t, std::uint32_t> destroy(std::uint32_t index) noexcept;
      std::byte& get(std::uint32_t index) const noexcept;

      std::byte* begin() const noexcept {
        return data_.get();
      }

      std::byte* end() const noexcept {
        return data_.get() + pool_end_;
      }

    private:
      std::size_t pool_end_ {0};
      std::size_t end_ {0};
      std::unique_ptr<std::byte, void(*)(std::byte*)> data_;

  };
}

#endif

