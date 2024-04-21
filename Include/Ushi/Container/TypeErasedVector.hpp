#ifndef USHI_INCLUDE_USHI_CONTAINER_TYPE_ERASED_VECTOR_HPP
#define USHI_INCLUDE_USHI_CONTAINER_TYPE_ERASED_VECTOR_HPP

#include "Core/Core.hpp"

namespace ushi {
namespace internal {
namespace container {

class TypeErasedVector {
  //   using TElementShape = ;

  template <u64 bytes> //
  struct Element {
    std::array<std::byte, bytes> data;
  };

public:
  TypeErasedVector() noexcept = default;

  constexpr TypeErasedVector(u64 elementSize)
      : m_elementSize{elementSize},
        m_sample{std::make_unique_for_overwrite<std::byte[]>(elementSize)},
        m_data{} {}

  constexpr auto size() const noexcept { return m_data.size() / m_elementSize; }

  constexpr auto push(auto &&value) {
    if (sizeof(value) != m_elementSize) {
      throw;
    }
    // m_sample->s = reinterpret_cast<std::byte[]>(std::forward(value));
    // m_data.insert
    // std::swap_ranges();
  }

private:
  u64 m_elementSize = 1;
  std::unique_ptr<std::byte[]> m_sample = nullptr;
  std::vector<std::byte> m_data = {};
};

} // namespace container
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_CONTAINER_TYPE_ERASED_VECTOR_HPP
