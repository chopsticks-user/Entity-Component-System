#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Container/TypeErasedVector.hpp"
#include "Entity/Entity.hpp"
#include "GroupImpl.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig TConfig> //
class GroupNetwork {
  using TCompatibleEntityIDs = container::UnorderedDenseMap<EntityID, EntityID>;
  using TSignature = TConfig::SignatureType;
  using TGroup = Group<TConfig>;
  using TArchetype =
      std::unordered_map<ComponentRecordID, container::TypeErasedVector>;
  using TGroupMap = std::unordered_map<TSignature, Group<TConfig>>;
  using TArchetypeInfo = std::vector<std::pair<ComponentRecordID, u64>>;
  using TPackage = std::pair<TCompatibleEntityIDs, TArchetype>;

  using TComponentRecord = ComponentRecord<TConfig>;
  using TEntity = Entity<TConfig>;

public:
  template <IsComponent... TComponents> //
  constexpr auto addEntityWith(const TComponentRecord &componentRecord,
                               const EntityID &entityID,
                               TComponents... components) -> void {
    m_groupMap[componentRecord.template signature<TComponents...>()]
        .template addEntityWith<TComponents...>(componentRecord, entityID,
                                                std::move(components)...);
  }

  template <IsComponent... TComponents> //
  constexpr auto addEntityWith(const TComponentRecord &componentRecord,
                               const std::vector<TEntity> &entities,
                               TComponents... components) -> void {
    m_groupMap[componentRecord.template signature<TComponents...>()]
        .template addEntityWith<TComponents...>(componentRecord, entities,
                                                std::move(components)...);
  }

  template <IsComponent... TComponents> //
  constexpr auto modifyComponentsOf(const TComponentRecord &componentRecord,
                                    const EntityID &entityID,
                                    TComponents... components) -> void {
    m_groupMap[componentRecord.template signature<TComponents...>()]
        .template modifyComponentsOf<TComponents...>(componentRecord, entityID,
                                                     std::move(components)...);
  }

  template <IsComponent... TComponents> //
  constexpr auto modifyComponentsOf(const TComponentRecord &componentRecord,
                                    const std::vector<TEntity> &entities,
                                    TComponents... components) -> void {
    m_groupMap[componentRecord.template signature<TComponents...>()]
        .template modifyComponentsOf<TComponents...>(componentRecord, entities,
                                                     std::move(components)...);
  }

  // * For testing purposes
  constexpr auto validateGroup(const TSignature &expectedSignature) -> bool {
    return m_correctGroupSignature(expectedSignature);
  }

private:
  //   template <IsComponent... TComponents> //
  //   constexpr auto m_tryContructGroup(const TComponentRecord
  //   &componentRecord)
  //       -> void {}

  constexpr auto m_correctGroupSignature(const TSignature &expectedSignature)
      -> bool {
    // * Queried group must exist
    TArchetypeInfo info = m_groupMap.at(expectedSignature).archetypeInfo();
    TSignature actualSignature{};
    for (const auto &[componentID, componentSize] : info) {
      actualSignature.set(componentID);
    }
    return actualSignature == expectedSignature;
  }

private:
  TGroupMap m_groupMap = {};
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP