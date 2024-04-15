#ifndef USHI_INCLUDE_USHI_IMPL_ENTITY_ID_HPP
#define USHI_INCLUDE_USHI_IMPL_ENTITY_ID_HPP

#include "Core/Core.hpp"

namespace ushi {
namespace internal {
namespace impl {

using EntityID = u64;

template <typename T_EID>
concept IsEID = core::IsHashable<T_EID>; // compareable, etc

/**
 * @brief
 *
 */
template <IsEID T_EID> //
class EntityIDGenerator final {
public:
  constexpr auto operator()() noexcept -> T_EID {
    return EntityID{m_current++};
  }

private:
  T_EID m_current = 0;
};

template <class T_EIDGenerator>
concept IsEIDGenerator =
    IsEID<typename core::FirstTemplateArg<T_EIDGenerator>::Type> && requires {
      {
        T_EIDGenerator{}()
      } -> std::convertible_to<
          typename core::FirstTemplateArg<T_EIDGenerator>::Type>;
    };

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_ENTITY_ID_HPP
