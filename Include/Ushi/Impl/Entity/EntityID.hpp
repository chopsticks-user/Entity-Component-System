#ifndef USHI_INCLUDE_USHI_IMPL_ENTITY_ID_HPP
#define USHI_INCLUDE_USHI_IMPL_ENTITY_ID_HPP

#include "Ushi/Core/Core.hpp"

#include <utility>

namespace ushi {

// * EntityID must be hashable
using EntityID = u64;

class EntityIDGenerator {
public:
  constexpr auto operator()() noexcept -> EntityID {
    return EntityID{m_current++};
  }

private:
  u64 m_current = 0;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_ENTITY_ID_HPP
