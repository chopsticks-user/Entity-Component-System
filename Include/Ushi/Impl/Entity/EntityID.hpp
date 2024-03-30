#ifndef USHI_INCLUDE_USHI_IMPL_ENTITY_ID_HPP
#define USHI_INCLUDE_USHI_IMPL_ENTITY_ID_HPP

#include "Ushi/Core/Core.hpp"

namespace ushi {

using EntityID = u64;

template <typename T_EID>
concept IsEID = IsHashable<T_EID>; // compareable, etc

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
    IsEID<typename FirstTemplateArg<T_EIDGenerator>::Type> && requires {
      {
        T_EIDGenerator{}()
      } -> std::convertible_to<typename FirstTemplateArg<T_EIDGenerator>::Type>;
    };

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_ENTITY_ID_HPP
