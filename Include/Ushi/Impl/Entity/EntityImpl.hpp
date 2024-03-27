#ifndef TORA_INCLUDE_TORA_IMPL_ENTITY_IMPL_HPP
#define TORA_INCLUDE_TORA_IMPL_ENTITY_IMPL_HPP

#include "Core/Core.hpp"

namespace ushi {

class Entity {
  friend class EntityFactory;

public:
  constexpr auto id() const noexcept -> EntityID { return m_id; }

private:
  EntityID m_id;
  World &m_world;
};

} // namespace ushi

#endif // TORA_INCLUDE_TORA_IMPL_ENTITY_IMPL_HPP
