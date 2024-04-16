#ifndef USHI_INCLUDE_USHI_IMPL_SYSTEM_MANAGER_HPP
#define USHI_INCLUDE_USHI_IMPL_SYSTEM_MANAGER_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Entity/Entity.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig T_Config> //
class SystemManager final {
  using T_Signature = T_Config::SignatureType;
  using T_SystemTable = std::unordered_map<std::type_index, T_Signature>;
  // TODO: DenseSet & UnorderedDenseSet
  using T_Archetype =
      std::unordered_map<T_Signature, std::unordered_set<EntityID>>;

  using T_EntityManager = EntityManager<T_Config>;
  using T_Entity = Entity<T_Config>;

public:
  template <typename T_System, IsComponent... T_Components> //
  constexpr auto regster(const ComponentTable<T_Config> &componentTable)
      -> void {
    auto tpIndex = std::type_index{typeid(T_System)};
    if (m_systemTable.contains(tpIndex)) {
      return;
    }

    T_Signature systemSignature =
        componentTable.template signature<T_Components...>();
    m_systemTable[tpIndex] = systemSignature;
    if (m_archetype.contains(systemSignature)) {
      return;
    }
    auto compatibleEntityIDs =
        componentTable.template allEntitiesWith<T_Components...>();

    for (const auto &eid : compatibleEntityIDs) {
      m_archetype[systemSignature].insert(eid);
    }
  }

  template <typename T_System> //
  constexpr auto deregster() noexcept {
    m_systemTable.erase(std::type_index{typeid(T_System)});
  }

  // TODO:
  constexpr auto cleanUnusedArchetypes() noexcept {}

  template <typename T_System> //
  constexpr auto execute(ComponentTable<T_Config> &componentTable) const
      -> void {
    typename core::ArgumentTuple<decltype(T_System::function)>::Type
        sampleTuple{};

    for (const auto &entityID :
         m_archetype.at(m_systemTable.at(typeid(T_System)))) {
      auto args = componentTable.template componentsOf(entityID, sampleTuple);
      std::apply(T_System::function, args);
    }
  }

  constexpr auto update(const EntityID &eid, const T_Signature &entitySignature)
      -> void {
    for (auto &[archSignature, entityIDs] : m_archetype) {
      if ((entitySignature & archSignature) == archSignature) {
        entityIDs.insert(eid);
      } else {
        entityIDs.erase(eid);
      }
    }
  }

  constexpr auto update(const std::vector<T_Entity> &entities,
                        const T_Signature &entitySignature) -> void {
    for (const auto &entity : entities) {
      update(entity, entitySignature);
    }
  }

  constexpr auto forceRemove(const EntityID &eid) -> void {
    for (auto &[archSignature, entityIDs] : m_archetype) {
      entityIDs.erase(eid);
    }
  }

  constexpr auto forceRemove(const std::vector<T_Entity> &entities) -> void {
    for (const auto &entity : entities) {
      forceRemove(entity);
    }
  }

private:
  T_SystemTable m_systemTable;
  T_Archetype m_archetype;
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_SYSTEM_MANAGER_HPP