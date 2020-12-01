#ifndef ECS_OBJECT_POOL_H_
#define ECS_OBJECT_POOL_H_
#include <cstddef>
#include <memory>

namespace ecs {
  template<typename T, std::size_t N = 256>
  class object_pool {
    public:

      [[nodiscard]] T* create() {
        if(pool_end_ < end_) [[likely]] {
          return &data_.get()[pool_end_++];
        }
        return nullptr;
      }

      std::pair<std::size_t, std::size_t> destroy(std::size_t index) {
        std::swap(data_.get()[index], data_.get()[--pool_end_]);
        return {index, pool_end_};
      }

      [[nodiscard]] T* get(std::size_t index) const noexcept {
        if(index < end_) [[likely]] {
          return data_.get() + index;
        }
        return nullptr;
      }

      [[nodiscard]] inline std::size_t size() const noexcept {
        return pool_end_;
      }

      [[nodiscard]] inline T* begin() const noexcept {
        return data_.get();
      }

      [[nodiscard]] inline T* end() const noexcept {
        return data_.get() + pool_end_;
      }

      [[nodiscard]] inline T const* const cbegin() const noexcept {
        return data_.get();
      }

      [[nodiscard]] inline T const* const cend() const noexcept {
        return data_.get() + pool_end_;
      }
    private:
      std::unique_ptr<T, void(*)(T* data)> data_ { new T[N], [](T* data){delete[] data;}};
      std::size_t end_ {N};
      std::size_t pool_end_ {0};
 };
}
#endif

