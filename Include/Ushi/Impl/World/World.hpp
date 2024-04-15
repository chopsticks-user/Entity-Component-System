#ifndef USHI_INCLUDE_USHI_IMPL_WORLD_HPP
#define USHI_INCLUDE_USHI_IMPL_WORLD_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Entity/Entity.hpp"
#include "System/System.hpp"

namespace ushi {

template <IsConfig T_Config = DefaultConfig> //
class World final {
  using T_EntityManager = EntityManager<T_Config>;
  using T_ComponentTable = ComponentTable<T_Config>;
  using T_SystemManager = SystemManager<T_Config>;

  using T_Signature = T_Config::SignatureType;

public:
  static auto instance() -> std::shared_ptr<World>;

private:
  constexpr World() = default;

public:
  //===============================Entity======================================

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto create(T_Components... components) -> EntityID {
    auto entityID =
        m_entityManager.template create(signatureOf<T_Components...>());
    m_componentTable.template addEntityWith<T_Components...>(
        entityID, std::move(components)...);
    return entityID;
  }

  // template <IsComponent... T_Components> //
  // [[nodiscard]] constexpr auto create() -> EntityID {
  //   return create<T_Components...>(T_Components{}, ...);
  // }

  template <IsComponent... T_Components> //
  constexpr auto addComponentsToEntity(const EntityID &entityID,
                                       T_Components... components) -> void {
    if (!m_entityManager.contains(entityID)) {
      throw std::runtime_error("Entity not existed");
    }

    m_entityManager.setSignature(entityID, signatureOf<T_Components...>());
    m_componentTable.template addEntityWith<T_Components...>(
        entityID, std::move(components)...);
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto hasComponents(const EntityID &entityID) -> bool {
    auto componentPackSignature = signatureOf<T_Components...>();
    return (m_entityManager.template getSignature(entityID) &
            componentPackSignature) == componentPackSignature;
  }

  //===============================Component===================================

  template <IsComponent... T_Components> //
  constexpr auto record() -> void {
    (m_componentTable.template regster<T_Components>(), ...);
  }

  constexpr auto nMaxComponents() const noexcept -> u64 {
    return T_ComponentTable::maxComponents;
  }

  constexpr auto nRecordedComponents() const noexcept -> u64 {
    return m_componentTable.template componentTypeCount();
  }

  constexpr auto componentRecordFull() const noexcept -> bool {
    return m_componentTable.template recordFull();
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto signatureOf() const noexcept -> T_Signature {
    return m_componentTable.template signature<T_Components...>();
  }

  //===============================System======================================

private:
  T_EntityManager m_entityManager = {};
  T_ComponentTable m_componentTable = {};
  T_SystemManager m_systemManager = {};
};

template <IsConfig T_Config>
auto World<T_Config>::instance() -> std::shared_ptr<World> {
  static std::atomic<std::shared_ptr<World>> worldInstance = nullptr;

  if (worldInstance.load() == nullptr) {
    worldInstance.store(std::shared_ptr<World>(new World{}));
  }
  return worldInstance.load();
}

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_WORLD_HPP
