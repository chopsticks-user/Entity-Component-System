#ifndef TORA_INCLUDE_TORA_WORLD_HPP
#define TORA_INCLUDE_TORA_WORLD_HPP

#if __cplusplus >= 202002L
#include "Base.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "System.hpp"

namespace tora {

class World {
public:
  //* =============================== Component ===============================

  /**
   * @throws std::bad_alloc from ecs::ComponentTable::reg.
   */
  template <typename ComponentType> //
  void registerComponent() noexcept(false)
    requires CValidComponent<ComponentType>
  {
    this->mComponentTable->reg<ComponentType>();
    this->mEntityManager->setNComponents(
        this->mComponentTable->getNComponents());
  }

  // TODO: might need a method to deregister a component

  /**
   * @throws std::out_of_range from ecs::ComponentTable::get.
   */
  template <typename ComponentType> //
  ComponentType &getComponent(u64 entityID) noexcept(false)
    requires CValidComponent<ComponentType>
  {
    return this->mComponentTable->get<ComponentType>(entityID);
  }

  /**
   * @throws std::out_of_range from ecs::ComponentTable::get.
   */
  template <typename ComponentType, typename EntityType> //
  ComponentType &getComponent(const EntityType &entity) noexcept(false)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    return this->getComponent<ComponentType>(entity.getID());
  }

  /**
   * @throws std::bad_alloc or std::out_of_range.
   */
  template <typename... ComponentTypes> //
  void addComponents(u64 entityID,
                     ComponentTypes... components) noexcept(false) {
    auto argTuple = std::make_tuple(std::move(components)...);
    iterateTuple<0, ComponentTypes...>(argTuple, [&](auto component) {
      this->mComponentTable->add<decltype(component)>(std::move(component));
    });

    auto newSignature =
        this->mComponentTable->querySignature<ComponentTypes...>();
    this->mSystemManager->update(entityID, std::move(newSignature));
    this->mEntityManager->setSignature(entityID, newSignature);
  }

  /**
   * @throws std::bad_alloc or std::out_of_range.
   */
  template <typename... ComponentTypes, typename EntityType> //
  void addComponents(const EntityType &entity,
                     ComponentTypes... components) noexcept(false)
    requires CValidEntity<EntityType>
  {
    auto argTuple = std::make_tuple(std::move(components)...);
    iterateTuple<0, ComponentTypes...>(argTuple, [&](auto component) {
      this->mComponentTable->add<decltype(component)>(entity.getID(),
                                                      std::move(component));
    });

    auto newSignature =
        this->mComponentTable->querySignature<ComponentTypes...>();
    this->mSystemManager->update(entity.getID(), newSignature);
    this->mEntityManager->setSignature(entity.getID(), std::move(newSignature));
  }

  /**
   * @throws std::bad_alloc or std::out_of_range.
   */
  template <typename ComponentType> //
  void addComponent(u64 entityID, ComponentType component = {}) noexcept(false)
    requires CValidComponent<ComponentType>
  {
    this->mEntityManager->setSignature(
        entityID, this->mComponentTable->getIndex<ComponentType>(), true);
    this->mComponentTable->add(entityID, std::move(component));
    this->mSystemManager->update(entityID,
                                 this->mEntityManager->getSignature(entityID));
  }

  /**
   * @throws std::bad_alloc or std::out_of_range.
   */
  template <typename ComponentType, typename EntityType> //
  void addComponent(const EntityType &entity,
                    ComponentType component = {}) noexcept(false)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->addComponent(entity.getID(), std::move(component));
  }

  /**
   * @throws std::bad_alloc or std::out_of_range.
   */
  template <typename ComponentType> //
  void removeComponent(u64 entityID) noexcept(false)
    requires CValidComponent<ComponentType>
  {
    this->mEntityManager->setSignature(
        entityID, this->mComponentTable->getIndex<ComponentType>(), false);
    this->mComponentTable->remove<ComponentType>(entityID);
    this->mSystemManager->update(entityID,
                                 this->mEntityManager->getSignature(entityID));
  }

  /**
   * @throws std::bad_alloc or std::out_of_range.
   */
  template <typename ComponentType, typename EntityType> //
  void removeComponent(const EntityType &entity) noexcept(false)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->removeComponent<ComponentType>(entity.getID());
  }

  /**
   * @throws std::bad_alloc or std::out_of_range.
   */
  template <typename... ComponentTypes> //
  void removeComponents(u64 entityID) noexcept(false) {
    auto argTuple = std::tuple<ComponentTypes...>();
    iterateTuple<0, ComponentTypes...>(argTuple, [&](auto component) {
      this->mComponentTable->remove<decltype(component)>(entityID);
      this->mEntityManager->setSignature(
          entityID, this->mComponentTable->getIndex<decltype(component)>(),
          false);
    });

    this->mSystemManager->update(entityID,
                                 this->mEntityManager->getSignature(entityID));
  }

  /**
   * @throws std::bad_alloc or std::out_of_range.
   */
  template <typename... ComponentTypes, typename EntityType> //
  void removeComponents(const EntityType &entity) noexcept(false)
    requires CValidEntity<EntityType>
  {
    this->removeComponents<ComponentTypes...>(entity.getID());
  }

  //* =============================== Entity ==================================

  /**
   * @throws std::out_of_range if entityID is not registered.
   */
  template <typename EntityType> //
  EntityType getEntity(u64 entityID) noexcept(false)
    requires CValidEntity<EntityType>
  {
    return this->mEntityManager->get<EntityType>(entityID);
  }

  /**
   * @throws std::out_of_range if entityID is not registered.
   */
  template <typename EntityType> //
  EntityType addEntity() noexcept(false)
    requires CValidEntity<EntityType>
  {
    return this->mEntityManager->add<EntityType>();
  }

  // TODO: add multiple entities in one call

  // TODO: make this->mSystemManager->remove(entityID) noexcept
  void removeEntity(u64 entityID) noexcept {
    this->mEntityManager->remove(entityID);
    this->mComponentTable->remove(entityID);
    this->mSystemManager->remove(entityID);
  }

  template <typename EntityType> //
  void removeEntity(EntityType &entity) noexcept
    requires CValidEntity<EntityType>
  {
    this->removeEntity(entity.getID());
  }

  // TODO: remove multiple entities in one call

  //* =============================== System ==================================

  /**
   * @brief System must be static, that is, the qualified condition does not
   * change. The second registeration of a system type will be ignore.
   * @throws std::bad_alloc from ecs::SystemManager::reg
   */
  template <typename SystemType, typename... RequiredComponentTypes> //
  void registerSystem() noexcept(false)
    requires CValidSystem<SystemType>
  {
    this->mSystemManager->reg<SystemType>(std::move(
        this->mComponentTable->querySignature<RequiredComponentTypes...>()));
  }

  // TODO: might need a method to deregister a system

  /**
   * @throws std::out_of_range from ecs::SystemManager::execute and any
   * exceptions thrown by user's implementation of System::function.
   */
  template <typename SystemType, typename... Args> //
  void execute(Args &&...args) noexcept(false)
    requires CValidSystem<SystemType>
  {
    this->mSystemManager->execute<SystemType>(*this,
                                              std::forward<Args>(args)...);
  }

  // TODO: change signature of a registered system

  //* =========================================================================

  void clear() noexcept {
    this->mEntityManager->clear();
    this->mComponentTable->clear();
    this->mSystemManager->clear();
  }

private:
  /**
   * @throws std::out_of_range from ecs::System::getQualifications
   */
  template <typename SystemType> //
  const DynamicBitset &getSystemSignature() const noexcept(false)
    requires CValidSystem<SystemType>
  {
    return this->mSystemManager->getQualifications<SystemType>();
  }

  /**
   * @throws std::out_of_range from ecs::EntityManager::getSignature
   */
  const DynamicBitset &getEntitySignature(u64 entityID) const noexcept(false) {
    return this->mEntityManager->getSignature(entityID);
  }

private:
  std::unique_ptr<EntityManager> mEntityManager =
      std::make_unique<EntityManager>();
  std::unique_ptr<ComponentTable> mComponentTable =
      std::make_unique<ComponentTable>();
  std::unique_ptr<SystemManager> mSystemManager =
      std::make_unique<SystemManager>();
};

} // namespace tora
#endif // C++20 or newer

#endif // TORA_INCLUDE_TORA_WORLD_HPP