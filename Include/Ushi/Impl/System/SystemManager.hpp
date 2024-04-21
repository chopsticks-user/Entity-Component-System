#ifndef USHI_INCLUDE_USHI_IMPL_SYSTEM_MANAGER_HPP
#define USHI_INCLUDE_USHI_IMPL_SYSTEM_MANAGER_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Entity/Entity.hpp"
#include "Group/Group.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig TConfig> //
class SystemManager final {
  using TSignature = TConfig::SignatureType;
  using TSystemTable = std::unordered_map<std::type_index, TSignature>;
  // TODO: DenseSet & UnorderedDenseSet
  // using TArchetype =
  //     std::unordered_map<TSignature, std::unordered_set<EntityID>>;

  using TGroupNetwork = GroupNetwork<TConfig>;
  using TEntityManager = EntityManager<TConfig>;
  using TEntity = Entity<TConfig>;

public:
  // TODO: IsComponent... TComponents is not needed
  template <typename TSystem, IsComponent... TComponents> //
  constexpr auto regster(const ComponentTable<TConfig> &componentTable)
      -> void {
    // typename core::ArgumentTuple<decltype(TSystem::function)>::Type
    //     sampleTuple{};

    auto tpIndex = std::type_index{typeid(TSystem)};
    if (m_systemTable.contains(tpIndex)) {
      return;
    }

    m_systemTable[tpIndex] =
        componentTable.template signature<TComponents...>();

    // if (m_archetype.contains(systemSignature)) {
    //   return;
    // }
    // auto compatibleEntityIDs =
    //     componentTable.template allEntitiesWith<TComponents...>();

    // m_archetype[systemSignature] = {};
    // for (const auto &eid : compatibleEntityIDs) {
    //   m_archetype[systemSignature].insert(eid);
    // }
  }

  template <typename TSystem> //
  constexpr auto deregster() noexcept {
    m_systemTable.erase(std::type_index{typeid(TSystem)});
  }

  template <typename TSystem> //
  constexpr auto execute(const TGroupNetwork &groupNetwork) const -> void {
    typename core::ArgumentTuple<decltype(TSystem::function)>::Type
        sampleTuple{};

    auto entityView =
        groupNetwork.entityView(m_systemTable.at(typeid(TSystem), sampleTuple));

    // auto args = componentTable.template componentsOf(entityID,
    // sampleTuple); std::apply(TSystem::function, args);
  }

  constexpr auto update(const EntityID &eid, const TSignature &entitySignature)
      -> void {
    // for (auto &[archSignature, entityIDs] : m_archetype) {
    //   if ((entitySignature & archSignature) == archSignature) {
    //     entityIDs.insert(eid);
    //   } else {
    //     entityIDs.erase(eid);
    //   }
    // }
  }

  constexpr auto update(const std::vector<TEntity> &entities,
                        const TSignature &entitySignature) -> void {
    // for (const auto &entity : entities) {
    //   update(entity, entitySignature);
    // }
  }

  constexpr auto forceRemove(const EntityID &eid) -> void {
    // for (auto &[archSignature, entityIDs] : m_archetype) {
    //   entityIDs.erase(eid);
    // }
  }

  constexpr auto forceRemove(const std::vector<TEntity> &entities) -> void {
    // for (const auto &entity : entities) {
    //   forceRemove(entity);
    // }
  }

private:
  TSystemTable m_systemTable;
  // TArchetype m_archetype;
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_SYSTEM_MANAGER_HPP