#ifndef ECS_OBJECT_POOL_H_
#define ECS_OBJECT_POOL_H_
#include <cstddef>
#include <optional>
#include <memory>

namespace ecs {
  template<typename T, std::size_t N = 256>
  class object_pool {
    public:
      std::optional<T*> create() noexcept {
        if(end_ < pool_end_) [[likely]] {
          return {&data_.get()[end_++]};
        }
        return std::nullopt;
      }

      std::pair<std::size_t, std::size_t> destroy(std::size_t index) noexcept {
        std::swap(data_.get()[index], data_.get()[--end_]);
        return {index, end_};
      }

      std::optional<T*> get(std::size_t index) noexcept {
        if(index < end_) [[likely]] {
          return {&data_.get()[index]};
        }

        return std::nullopt;
      }

      inline std::size_t size() const noexcept {
        return end_;
      }

      inline T* begin() const noexcept {
        return data_.get();
      }

      inline T const* const cbegin() const noexcept {
        return data_.get();
      }

      inline T* end() const noexcept {
        return &data_.get()[end_];
      }

      inline T const* const cend() const noexcept {
        return &data_.get()[end_];
      }

    private:
      std::unique_ptr<T, void(*)(T*)> data_ {new T[N], [](T* data) {delete[] data;}};
      std::size_t pool_end_ {N};
      std::size_t end_ {0};
  };
}
#endif

