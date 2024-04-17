#ifndef USHI_INCLUDE_USHI_IMPL_ENTITY_MANAGER_HPP
#define USHI_INCLUDE_USHI_IMPL_ENTITY_MANAGER_HPP

#include "Ushi/Core/Core.hpp"

#include "Container/UDenseMap.hpp"
#include "EntityFactory.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig T_Config> //
class EntityManager final {
  using T_Entity = Entity<T_Config>;
  using T_EIDGenerator = T_Config::EIDGeneratorType;

public:
  constexpr auto create(T_EntitySignature<T_Entity> entitySignature = {})
      -> T_Entity {
    T_Entity entity =
        m_entityFactory.template create<T_Entity>(entitySignature);
    m_entities.add(entity.id(), entity);
    return m_entities[entity.id()];
  }

  constexpr auto clone(const T_Entity &entity) -> T_Entity {
    T_Entity copy = m_entityFactory.template clone<T_Entity>(entity);
    m_entities.add(copy.id(), copy);
    return m_entities[copy.id()];
  }

  constexpr auto clone(const EntityID &entityID) -> T_Entity {
    return clone(m_entities[entityID]);
  }

  constexpr auto contains(const EntityID &entityID) const noexcept -> bool {
    return m_entities.contains(entityID);
  }

  constexpr auto get(const EntityID &entityID) const -> T_Entity {
    return m_entities[entityID];
  }

  constexpr auto destroy(EntityID &&entityID) noexcept -> void {
    m_entities.remove(std::move(entityID));
  }

  constexpr auto nEntities() const noexcept -> u64 { return m_entities.size(); }

  constexpr auto getSignature(const EntityID &entityID) const noexcept
      -> const T_Config::SignatureType & {
    return m_entities[entityID].signature();
  }

  constexpr auto setSignature(const EntityID &entityID,
                              T_EntitySignature<T_Entity> signature) -> void {
    m_entityFactory.setSignature(m_entities[entityID], std::move(signature));
  }

  constexpr auto setSignatureBit(const EntityID &entityID, u64 bitIndex,
                                 bool bitValue = true) -> void {
    m_entityFactory.setSignatureBit(m_entities[entityID], bitIndex, bitValue);
  }

private:
  EntityFactory<T_EIDGenerator> m_entityFactory = {};
  container::UDenseMap<EntityID, T_Entity> m_entities = {};
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // __USHI_INCLUDE_USHI_IMPL_ENTITY_MANAGER_HPP