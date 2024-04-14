#ifndef USHI_INCLUDE_USHI_CONTAINER_VECTOR_WRAPPER_HPP
#define USHI_INCLUDE_USHI_CONTAINER_VECTOR_WRAPPER_HPP

#include "Core/Core.hpp"

namespace ushi {

class VectorWrapperBase {
public:
  virtual ~VectorWrapperBase() noexcept = default;

  // virtual auto remove(u64 index) noexcept -> void = 0;

  // virtual auto perform(u64 index, std::function<void()> action) -> void = 0;
};

template <typename T> //
class VectorWrapper final : public VectorWrapperBase {
public:
  constexpr auto get() noexcept -> std::vector<T> & { return m_vector; }

  constexpr auto remove(u64 index) noexcept -> T {
    std::swap(m_vector[index], m_vector[m_vector.size() - 1]);
    T value = std::move(m_vector[m_vector.size() - 1]); //! Might be illformed
    m_vector.pop_back();
    return value;
  }

private:
  std::vector<T> m_vector;
};

template <typename T> //
using Vector = VectorWrapper<T>;

} // namespace ushi

#endif // USHI_INCLUDE_USHI_CONTAINER_VECTOR_WRAPPER_HPP