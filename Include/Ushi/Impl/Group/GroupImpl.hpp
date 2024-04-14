#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Container/UnorderedDenseMap.hpp"
#include "Container/VectorWrapper.hpp"
#include "Entity/Entity.hpp"

namespace ushi {

template <IsConfig T_Config> //
class GroupFactory;

template <IsConfig T_Config> //
class Group final {
  using T_GroupPtr = std::shared_ptr<Group<T_Config>>;
  using T_EntityIDContainer = std::vector<EntityID>;
  using T_ComponentTable =
      UnorderedDenseMap<u64, std::shared_ptr<VectorWrapperBase>>;
  using T_Signature = T_Config::SignatureType;
  using T_ChildrenGroups = std::vector<T_GroupPtr>;

  friend class GroupFactory<T_Config>;

public:
  constexpr Group(Group &&) noexcept = default;

  Group(const Group &) = delete;

  constexpr auto operator=(Group &&) noexcept -> Group & = default;

  auto operator=(const Group &) -> Group & = delete;

public:
  [[nodiscard]] constexpr auto level() const noexcept -> u64 {
    return m_table.size();
  }

  [[nodiscard]] constexpr auto signature() const noexcept
      -> const T_Signature & {
    return m_signature;
  }

  constexpr auto qualifiedChildren(const T_Signature &signature) const noexcept
      -> std::list<T_GroupPtr> {
    std::list<T_GroupPtr> qualified;
    for (auto &child : m_childrenGroups) {
      if ((child->signature() & signature) == signature) {
        qualified.push_back(child);
      }
    }
    return qualified;
  }

  template <IsComponent... T_Components>
  constexpr auto transfer(const ComponentRecord<T_Config> &componentRecord,
                          const EntityManager<T_Config> &entityManager,
                          const T_Signature &signature) noexcept
      -> std::pair<T_EntityIDContainer, T_ComponentTable> {
    std::pair<T_EntityIDContainer, T_ComponentTable> transferredData{};

    for (const auto &eid : m_entityIDs) {
      if ((entityManager.getSignature(eid) & signature) == signature) {
        transferredData.first.push_back(eid);
      }
    }

    (transferredData.second.add(
         componentRecord.template getIndex<T_Components>(),
         std::make_shared<VectorWrapperBase>(
             m_transfer(componentRecord.template getIndex<T_Components>(),
                        transferredData.first))),
     ...);

    return transferredData;
  }

private:
  template <IsComponent T_Component>
  constexpr auto m_transfer(const ComponentRecordID &componentID,
                            const std::vector<u64> &indices) noexcept
      -> VectorWrapper<T_Component> {
    VectorWrapper<T_Component> transferredComponents{};
    transferredComponents.get().reserve(indices.size());

    VectorWrapper<T_Component> &pComponents =
        *std::static_pointer_cast<VectorWrapper<T_Component>>(
            m_table[componentID]);

    for (const auto &index : indices) {
      transferredComponents.get().push_back(
          std::move(pComponents.remove(index)));
    }

    return transferredComponents;
  }

  Group() = default;

private:
  T_EntityIDContainer m_entityIDs;
  T_ComponentTable m_table;
  T_Signature m_signature;
  T_ChildrenGroups m_childrenGroups;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
