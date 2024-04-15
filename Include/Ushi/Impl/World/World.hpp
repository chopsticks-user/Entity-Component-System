#ifndef USHI_INCLUDE_USHI_IMPL_WORLD_HPP
#define USHI_INCLUDE_USHI_IMPL_WORLD_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Entity/Entity.hpp"
#include "System/System.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig T_Config = DefaultConfig> //
class World final {
  using T_EntityManager = EntityManager<T_Config>;
  using T_ComponentTable = ComponentTable<T_Config>;
  using T_SystemManager = SystemManager<T_Config>;

  using T_Signature = T_Config::SignatureType;
  using T_Entity = Entity<T_Config>;

public:
  static auto instance() -> std::shared_ptr<World>;

private:
  constexpr World() = default;

public:
  //===============================Entity======================================

  [[nodiscard]] constexpr auto create() -> T_Entity {
    return m_entityManager.template create();
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto create(T_Components... components) -> T_Entity {
    T_Entity entity =
        m_entityManager.template create(signatureOf<T_Components...>());
    m_componentTable.template addEntityWith<T_Components...>(
        entity, std::move(components)...);
    return entity;
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto
  create(std::tuple<T_Components...> componentTuple = {}) -> T_Entity {
    T_Entity entity =
        m_entityManager.template create(signatureOf<T_Components...>());

    std::apply(
        [&](auto &&...args) {
          m_componentTable.template addEntityWith<T_Components...>(entity.id(),
                                                                   args...);
        },
        componentTuple);
    return entity;
  }

  [[nodiscard]] constexpr auto create(u64 nEntities) -> std::vector<T_Entity> {
    std::vector<T_Entity> entities{};
    entities.reserve(nEntities);

    for (u64 i = 0; i < nEntities; ++i) {
      entities.push_back(m_entityManager.template create());
    }

    return entities;
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto
  create(u64 nEntities, std::tuple<T_Components...> componentTuple = {})
      -> std::vector<T_Entity> {
    std::vector<T_Entity> entities{};
    entities.reserve(nEntities);

    for (u64 i = 0; i < nEntities; ++i) {
      entities.push_back(
          m_entityManager.template create(signatureOf<T_Components...>()));
      std::apply(
          [&](auto &&...args) {
            m_componentTable.template addEntityWith<T_Components...>(
                entities[i].id(), args...);
          },
          componentTuple);
    }

    return entities;
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto create(u64 nEntities, T_Components... components)
      -> std::vector<T_Entity> {
    return create(nEntities, std::make_tuple(components...));
  }

  template <IsComponent... T_Components> //
  constexpr auto addComponents(const EntityID &entityID,
                               T_Components... components) -> void {
    if (!m_entityManager.contains(entityID)) {
      throw std::runtime_error("Entity not existed");
    }

    m_entityManager.setSignature(entityID, signatureOf(entityID) |
                                               signatureOf<T_Components...>());
    m_componentTable.template addEntityWith<T_Components...>(
        entityID, std::move(components)...);
  }

  // TODO: adding in batches efficiently
  template <IsComponent... T_Components> //
  constexpr auto addComponents(const std::vector<T_Entity> &entities,
                               T_Components... components) -> void {
    for (const auto &entity : entities) {
      addComponents<T_Components...>(entity, components...);
    }
  }

  template <IsComponent... T_Components> //
  constexpr auto addComponents(const std::vector<T_Entity> &entities,
                               std::tuple<T_Components...> componentTuple = {})
      -> void {
    std::apply(
        [&](auto &&...args) {
          addComponents<T_Components...>(entities, args...);
        },
        componentTuple);
  }

  // template <IsComponent... T_Components> //
  // constexpr auto removeComponentsOf(const EntityID &entityID,
  //                                   T_Components... components) -> void {
  //   if (!m_entityManager.contains(entityID)) {
  //     throw std::runtime_error("Entity not existed");
  //   }

  //   m_entityManager.setSignature(entityID, signatureOf(entityID) |
  //                                              signatureOf<T_Components...>());
  //   m_componentTable.template addEntityWith<T_Components...>(
  //       entityID, std::move(components)...);
  // }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto
  hasComponents(const EntityID &entityID) const noexcept -> bool {
    auto componentPackSignature = signatureOf<T_Components...>();
    return (m_entityManager.template getSignature(entityID) &
            componentPackSignature) == componentPackSignature;
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto
  hasComponents(const std::vector<T_Entity> &entities) const noexcept -> bool {
    bool result = false;
    for (const auto &entity : entities) {
      result |= hasComponents<T_Components...>(entity);
    }
    return result;
  }

  [[nodiscard]] constexpr auto
  signatureOf(const EntityID &entityID) const noexcept -> T_Signature {
    return m_entityManager.template getSignature(entityID);
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

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_WORLD_HPP
