#ifndef USHI_INCLUDE_USHI_IMPL_COMPONENT_TABLE_HPP
#define USHI_INCLUDE_USHI_IMPL_COMPONENT_TABLE_HPP

#include "Core/Core.hpp"

#include "Component.hpp"
#include "ComponentRecord.hpp"
#include "Config/Config.hpp"
#include "Container/UnorderedDenseMap.hpp"

namespace ushi {

template <IsConfig T_Config> //
class ComponentTable final {
  using T_Record = ComponentRecord<T_Config>;
  using T_ComponentRowBasePtr =
      std::shared_ptr<UnorderedDenseMapBase<EntityID>>;
  using T_Table = std::vector<T_ComponentRowBasePtr>;

  template <IsComponent T_Component>
  using T_ComponentRow = UnorderedDenseMap<EntityID, T_Component>;

  template <IsComponent T_Component>
  using T_ComponentRowPtr = std::shared_ptr<T_ComponentRow<T_Component>>;

public:
  constexpr auto componentTypeCount() -> u64 { return m_record.size(); }

  constexpr auto recordFull() -> bool { return m_record.full(); }

  template <IsComponent T_Component> //
  constexpr auto regster() -> void {
    m_record.template regster<T_Component>();
    m_table.resize(T_Record::maxComponents);
    m_table[m_record.template getIndex<T_Component>()] =
        std::make_shared<T_ComponentRow<T_Component>>();
  }

  template <IsComponent T_Component> //
  [[nodiscard]] constexpr auto contains() const noexcept -> bool {
    return m_record.contains();
  }

  template <IsComponent T_Component> //
  [[nodiscard]] constexpr auto componentRecordID() const noexcept -> u64 {
    return m_record.getIndex();
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto signature() const noexcept {
    return m_record.template signature<T_Components...>();
  }

public:
  template <IsComponent T_Component> //
  constexpr auto addEntityWith(const EntityID &eid, T_Component component)
      -> void {
    if (!m_record.template contains<T_Component>()) {
      throw std::runtime_error(
          "Unregistered component cannot be added to an entity");
    }

    m_componentRowPtr<T_Component>()->add(eid, std::move(component));
  }

  template <IsComponent... T_Components> //
  constexpr auto addEntityWith(const EntityID &eid, T_Components... components)
      -> void {
    (addEntityWith<T_Components>(eid, std::move(components)), ...);
  }

  template <IsComponent T_Component> //
  constexpr auto removeComponentsOf(const EntityID &eid) noexcept -> void {
    m_componentRowPtr<T_Component>()->remove(eid);
  }

  template <IsComponent... T_Components> //
  constexpr auto removeComponentsOf(const EntityID &eid) noexcept -> void {
    (m_componentRowPtr<T_Components>()->remove(eid), ...);
  }

  constexpr auto removeAllComponentsOf(const EntityID &eid) noexcept -> void {
    std::ranges::for_each(
        m_table, [&eid = std::as_const(eid)](T_ComponentRowBasePtr rowPtr) {
          rowPtr->remove(eid);
        });
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto componentsOf(const EntityID &eid)
      -> std::tuple<T_Components &...> {
    return std::forward_as_tuple(m_componentsOf<T_Components>(eid)...);
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto componentsOf(const EntityID &eid,
                                            const std::tuple<T_Components...> &)
      -> std::tuple<T_Components &...> {
    return componentsOf<T_Components...>(eid);
  }

  template <IsComponent T_FirstComponent> //
  [[nodiscard]] constexpr auto allEntitiesWith() const
      -> std::vector<EntityID> {
    return allEntitiesWith<T_FirstComponent>();
  }

  template <IsComponent T_FirstComponent, IsComponent... T_Components> //
  [[nodiscard]] constexpr auto allEntitiesWith() const
      -> std::vector<EntityID> {
    auto entityIDs = m_allEntitiesWith<T_FirstComponent>();
    (m_filterEntitiesWith<T_Components>(entityIDs), ...);
    return entityIDs;
  }

private:
  template <IsComponent T_Component> //
  [[nodiscard]] constexpr auto m_componentsOf(const EntityID &eid)
      -> T_Component & {
    return m_componentRowPtr<T_Component>()->at(eid);
  }

  template <IsComponent T_Component> //
  [[nodiscard]] constexpr auto m_allEntitiesWith() const
      -> std::vector<EntityID> {
    if (!m_record.template contains<T_Component>()) {
      throw std::runtime_error("Unregistered component cannot be accessed");
    }

    T_ComponentRowPtr<T_Component> componentRowPtr =
        m_componentRowPtr<T_Component>();
    return componentRowPtr->getKeys();
  }

  template <IsComponent T_Component> //
  [[nodiscard]] constexpr auto
  m_hasComponent(const EntityID &eid) const noexcept {
    if (!m_record.template contains<T_Component>()) {
      throw std::runtime_error("Unregistered component cannot be accessed");
    }

    return m_componentRowPtr<T_Component>()->contains(eid);
  }

  template <IsComponent T_Component> //
  constexpr auto m_filterEntitiesWith(std::vector<EntityID> &entitiyIDs) const
      -> void {
    T_ComponentRowPtr<T_Component> componentRowPtr =
        m_componentRowPtr<T_Component>();

    for (u64 i = 0; i < entitiyIDs.size(); ++i) {
      if (!componentRowPtr->contains(entitiyIDs[i])) {
        std::swap(entitiyIDs[i], entitiyIDs[entitiyIDs.size() - 1]);
        entitiyIDs.pop_back();
      }
    }
  }

  template <IsComponent T_Component> //
  [[nodiscard]] constexpr auto m_componentRowPtr()
      -> T_ComponentRowPtr<T_Component> {
    return std::static_pointer_cast<UnorderedDenseMap<EntityID, T_Component>>(
        m_table[m_record.template getIndex<T_Component>()]);
  }

  template <IsComponent T_Component> //
  [[nodiscard]] constexpr auto m_componentRowPtr() const
      -> T_ComponentRowPtr<T_Component> {
    return std::static_pointer_cast<UnorderedDenseMap<EntityID, T_Component>>(
        m_table[m_record.template getIndex<T_Component>()]);
  }

private:
  T_Record m_record;
  T_Table m_table;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_COMPONENT_TABLE_HPP