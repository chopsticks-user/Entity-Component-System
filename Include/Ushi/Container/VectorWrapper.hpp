#ifndef USHI_INCLUDE_USHI_CONTAINER_VECTOR_WRAPPER_HPP
#define USHI_INCLUDE_USHI_CONTAINER_VECTOR_WRAPPER_HPP

#include "Core/Core.hpp"

namespace ushi {

class VectorWrapperBase {
public:
  virtual ~VectorWrapperBase() noexcept = default;
};

template <typename T> //
class VectorWrapper final : public VectorWrapperBase {
public:
  constexpr auto operator()() noexcept -> std::vector<T> & { return m_vector; }

private:
  std::vector<T> m_vector;
};

template <typename T> //
using Vector = VectorWrapper<T>;

} // namespace ushi

#endif // USHI_INCLUDE_USHI_CONTAINER_VECTOR_WRAPPER_HPP