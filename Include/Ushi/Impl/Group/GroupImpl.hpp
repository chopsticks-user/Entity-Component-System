#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Container/TypeErasedVector.hpp"
#include "Container/UDenseMap.hpp"
#include "Entity/Entity.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig TConfig> //
class Group {
  // TODO: UDenseSet
  using TCompatibleEntityIDs = container::UDenseMap<EntityID, EntityID>;
  using TArchetype =
      std::unordered_map<ComponentRecordID, container::TypeErasedVector>;
  using TArchetypeInfo = std::vector<std::pair<ComponentRecordID, u64>>;
  using TPackage = std::pair<TCompatibleEntityIDs, TArchetype>;

  using TComponentRecord = ComponentRecord<TConfig>;

public:
  constexpr Group() noexcept = default;

  explicit constexpr Group(const TArchetypeInfo &archetypeInfo)
      : m_entityIDs{}, m_archetype{} {
    for (const auto &[componentID, elementSize] : archetypeInfo) {
      m_archetype.try_emplace(componentID,
                              container::TypeErasedVector{elementSize});
    }
  }

  explicit constexpr Group(const TCompatibleEntityIDs &entityIDs,
                           const TArchetype &archetype)
      : m_entityIDs{entityIDs}, m_archetype{archetype} {}

public:
  constexpr auto level() const noexcept -> u64 { return m_archetype.size(); }

  constexpr auto archetypeInfo() const noexcept -> TArchetypeInfo {
    TArchetypeInfo info(m_archetype.size());
    for (const auto &[componentID, componentVector] : m_archetype) {
      info.push_back(
          std::make_pair(componentID, componentVector.elementSize()));
    }
    return info;
  }

  template <IsComponent... TComponents> //
  constexpr auto addEntityWith(const TComponentRecord &componentRecord,
                               const EntityID &entityID,
                               std::tuple<TComponents...> &&components)
      -> void {
    // * For testing purposes
    if (m_entityIDs.contains(entityID)) {
      modifyComponents(componentRecord, entityID, components);
      return;
    }

    m_entityIDs.add(entityID, entityID);
    (m_archetype[componentRecord.template getIndex<TComponents>()].add(
         std::get<TComponents>(components)),
     ...);
  }

  template <IsComponent... TComponents> //
  constexpr auto modifyComponentsOf(const TComponentRecord &componentRecord,
                                    const EntityID &entityID,
                                    std::tuple<TComponents...> &&components)
      -> void {
    (m_archetype[componentRecord.template getIndex<TComponents>()].replace(
         m_entityIDs.indexOf(entityID), std::get<TComponents>(components)),
     ...);
  }

  // TODO: ensure move semantics
  // * Expect all elements in entityIDs are not present in m_entityIDs and
  // both entityIDs and archetype are compatible
  constexpr auto receive(TPackage &&package) -> void {
    for (auto &&entityID : package.first) {
      m_entityIDs.add(entityID, entityID);
    }
    for (auto &&[componentID, componentVector] : package.second) {
      m_archetype[componentID].add(componentVector);
    }
  }

  // TODO:
  template <IsComponent... TRemoveComponents> //
  constexpr auto shrinkThenTransfer(const TComponentRecord &componentRecord,
                                    const std::vector<EntityID> &entityIDs)
      -> TPackage {
    auto package = m_constructBackwardPackage<TRemoveComponents...>(
        componentRecord, entityIDs);

    for (const auto &entityID : entityIDs) {
      m_entityIDs.remove(entityID);
      auto entityIndex = m_entityIDs.indexOf(entityID);

      for (auto &[componentID, componentVector] : m_archetype) {
        package.second[componentID].add(componentVector.remove(entityIndex));
      }
    }
    return package;
  }

  // TODO:
  template <IsComponent... TAddComponents> //
  constexpr auto appendThenTransfer(const TComponentRecord &componentRecord,
                                    const std::vector<EntityID> &entityIDs,
                                    std::tuple<TAddComponents...> &&components)
      -> TPackage {
    auto package = m_constructForwardPackage<TAddComponents...>(componentRecord,
                                                                entityIDs);

    for (const auto &entityID : entityIDs) {
      m_entityIDs.remove(entityID);
      auto entityIndex = m_entityIDs.indexOf(entityID);

      for (auto &[componentID, componentVector] : m_archetype) {
        package.second[componentID].add(componentVector.remove(entityIndex));
      }

      (package.second[componentRecord.template getIndex<TAddComponents>()].add(
           std::get<TAddComponents>(components)),
       ...);
    }

    return package;
  }

private:
  template <IsComponent... TRemoveComponents> //
  constexpr auto
  m_constructBackwardPackage(const TComponentRecord &componentRecord,
                             const std::vector<EntityID> &entityIDs)
      -> TPackage {
    TPackage package{};

    for (const auto &entityID : entityIDs) {
      package.first.add(entityID, entityID);
    }

    for (const auto &[componentID, componentVector] : m_archetype) {
      package.second.try_emplace(
          componentID,
          container::TypeErasedVector{componentVector.elementSize()});
    }

    (package.second.erase(
         componentRecord.template getIndex<TRemoveComponents>()),
     ...);

    return package;
  }

  template <IsComponent... TAddComponents> //
  constexpr auto
  m_constructForwardPackage(const TComponentRecord &componentRecord,
                            const std::vector<EntityID> &entityIDs)
      -> TPackage {
    TPackage package{};

    for (const auto &entityID : entityIDs) {
      package.first.add(entityID, entityID);
    }

    for (const auto &[componentID, componentVector] : m_archetype) {
      package.second.try_emplace(
          componentID,
          container::TypeErasedVector{componentVector.elementSize()});
    }

    (package.second.try_emplace(
         componentRecord.template getIndex<TAddComponents>(),
         container::TypeErasedVector{sizeof(TAddComponents)}),
     ...);

    return package;
  }

private:
  TCompatibleEntityIDs m_entityIDs{};
  TArchetype m_archetype = {};
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP