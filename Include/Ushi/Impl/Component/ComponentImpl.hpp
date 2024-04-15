#ifndef USHI_INCLUDE_USHI_IMPL_COMPONENT_IMPL_HPP
#define USHI_INCLUDE_USHI_IMPL_COMPONENT_IMPL_HPP

#include "Core/Core.hpp"

namespace ushi {
namespace internal {
namespace impl {

struct Component {
  virtual ~Component() noexcept = default;
};

template <typename T_Component>
concept IsComponent = core::IsChildOf<T_Component, Component>;

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_COMPONENT_IMPL_HPP