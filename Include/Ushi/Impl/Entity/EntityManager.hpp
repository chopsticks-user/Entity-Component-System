#ifndef USHI_INCLUDE_USHI_IMPL_ENTITY_MANAGER_HPP
#define USHI_INCLUDE_USHI_IMPL_ENTITY_MANAGER_HPP

#include "Ushi/Core/Core.hpp"

#include "Ushi/Container/UnorderedDenseMap.hpp"
#include "Ushi/Impl/Entity/EntityFactory.hpp"

#include <unordered_set>

namespace ushi {

template <IsValidEntity T_Entity> //
class EntityManager {
public:
  constexpr auto create(T_EntitySignature<T_Entity> entitySignature = {})
      -> T_Entity {
    T_Entity entity = m_entityFactory.create<T_Entity>(entitySignature);
    m_entities.add(entity.id(), entity);
    return m_entities[entity.id()];
  }

  constexpr auto clone(const T_Entity &entity) -> T_Entity {
    T_Entity copy = m_entityFactory.clone<T_Entity>(entity);
    m_entities.add(copy.id(), copy);
    return m_entities[copy.id()];
  }

  constexpr auto clone(const EntityID &entityID) -> T_Entity {
    return clone(m_entities[entityID]);
  }

  constexpr auto contains(const EntityID &entityID) const noexcept -> bool {
    return m_entities.exists(entityID);
  }

  //   constexpr auto contains(const T_Entity &entity) const noexcept -> bool {
  //     return contains(entity.id());
  //   }

  constexpr auto get(const EntityID &entityID) const -> T_Entity {
    return m_entities[entityID];
  }

  constexpr auto destroy(EntityID &&entityID) noexcept -> void {
    m_entities.remove(std::move(entityID));
  }

  //   constexpr auto destroy(T_Entity &&entity) noexcept -> void {
  //     destroy(entity.id());
  //   }

  constexpr auto nEntities() const noexcept -> u64 { return m_entities.size(); }

  constexpr auto setSignature(const EntityID &entityID,
                              T_EntitySignature<T_Entity> signature) -> void {
    m_entityFactory.setSignature(m_entities[entityID], std::move(signature));
  }

  constexpr auto setSignatureBit(const EntityID &entityID, u64 bitIndex,
                                 bool bitValue = true) -> void {
    m_entityFactory.setSignatureBit(m_entities[entityID], bitIndex, bitValue);
  }

private:
  EntityFactory m_entityFactory = {};
  UnorderedDenseMap<EntityID, T_Entity> m_entities;
};

} // namespace ushi

#endif // __USHI_INCLUDE_USHI_IMPL_ENTITY_MANAGER_HPP