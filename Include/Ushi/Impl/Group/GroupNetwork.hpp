#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Entity/Entity.hpp"
#include "GroupImpl.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig TConfig> //
class GroupNetwork {
  using TSignature = TConfig::SignatureType;
  using TGroup = Group<TConfig>;
  using TArchetypeMap = std::unordered_map<TSignature, TGroup>;
  using TComponentPackage =
      std::unordered_map<std::type_index, std::unique_ptr<Component>>;
  using TArchetypeInfo = std::unordered_map<
      std::type_index,
      std::shared_ptr<container::VectorWrapperBase<Component>>>;
  using TComponentRecord = ComponentRecord<TConfig>;
  using TEntityManager = EntityManager<TConfig>;

public:
  template <IsComponent... TComponents> //
  constexpr auto addEntityWith(const TComponentRecord &componentRecord,
                               const EntityID &entityID,
                               TComponents... components) const noexcept
      -> void {
    auto signature = componentRecord.template signature<TComponents...>();
    m_tryInitializeEntity(signature, componentRecord);
    m_archetypeMap[signature].template addEntityWith<TComponents...>(
        entityID, std::move(components)...);
  }

  //! Use tuple instead
  // * Group with currentSignature must exist
  template <IsComponent... TAddComponents> //
  constexpr auto addComponentsTo(const TComponentRecord &componentRecord,
                                 const TEntityManager &entityManager,
                                 const EntityID &entityID,
                                 TAddComponents... components) const noexcept
      -> void {
    auto currentSignature = entityManager.getSignature(entityID);
    auto newSignature =
        componentRecord.template signature<TAddComponents...>() |
        currentSignature;

    if (!m_exists(newSignature)) {
      m_archetypeMap[newSignature] = TGroup{
          m_constructLargerTableWith<TAddComponents...>(currentSignature)};
    }

    // * Use .at for testing purposes
    auto componentPackage =
        m_archetypeMap.at(currentSignature).transfer(entityID);
    (componentPackage.insert_or_assign(typeid(TAddComponents),
                                       componentPackage),
     ...);
    m_archetypeMap[newSignature].receive(componentPackage);
  }

  // * Group with currentSignature must exist
  template <IsComponent... TRemoveComponents> //
  constexpr auto
  removeComponentsFrom(const TComponentRecord &componentRecord,
                       const TEntityManager &entityManager,
                       const EntityID &entityID,
                       TRemoveComponents... components) const noexcept -> void {
    auto currentSignature = entityManager.getSignature(entityID);
    auto newSignature =
        currentSignature ^
        (currentSignature &
         componentRecord.template signature<TRemoveComponents...>());

    if (!m_exists(newSignature)) {
      m_archetypeMap[newSignature] =
          TGroup{m_constructSmallerTableWithout<TRemoveComponents...>(
              currentSignature)};
    }

    auto componentPackage =
        m_archetypeMap.at(currentSignature).transfer(entityID);
    (componentPackage.erase(typeid(TRemoveComponents)), ...);
    m_archetypeMap[newSignature].receive(componentPackage);
  }

private:
  [[nodiscard]] constexpr auto m_exists(const TSignature &signature) -> bool {
    return m_archetypeMap.contains(signature);
  }

  constexpr auto m_tryInitializeEntity(const TComponentRecord &componentRecord,
                                       const TSignature &signature) -> void {
    if (m_exists(signature)) {
      return;
    }
    auto types = componentRecord.getTypesFromSignature(signature);
    TArchetypeInfo info{};
    for (auto &type : types) {
      info.try_emplace(type);
    }
    m_archetypeMap[signature] = TGroup{info};
  }

  // * Group with the given signature must exist
  template <IsComponent... TAddComponents> //
  [[nodiscard]] constexpr auto
  m_constructLargerTableWith(const TSignature &signature) const noexcept
      -> TArchetypeInfo {
    return m_archetypeMap[signature]
        .template constructLargerTableWith<TAddComponents...>();
  }

  // * Group with the given signature must exist
  template <IsComponent... TRemoveComponents> //
  [[nodiscard]] constexpr auto
  m_constructSmallerTableWithout(const TSignature &signature) const noexcept
      -> TArchetypeInfo {
    return m_archetypeMap[signature]
        .template constructSmallerTableWithout<TRemoveComponents...>();
  }

private:
  TArchetypeMap m_archetypeMap = {};
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP