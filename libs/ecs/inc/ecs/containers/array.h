#ifndef ECS_INC_ECS_CONTAINERS_ARRAY_H
#define ECS_INC_ECS_CONTAINERS_ARRAY_H
#include <cstdint>

namespace acorn {
namespace ecs {
namespace containers {
template<class T>
class array {
  public:
    using size_type = std::size_t;
    array() : data_(new T[256]) {}

    ~array() {
      delete[] data_;
   }

    [[nodiscard]] inline T& operator[](size_type index) const noexcept {
      return data_[index];
    }

    [[nodiscard]] inline T& get(size_type index) const noexcept {
      return data_[index];
    }

    T* begin() const noexcept {
      return data_;
    }

    T* end() const noexcept {
      return data_[255];
    }

  private:
    T* data_ {nullptr};
}; 

}
}
}
#endif

