#ifndef ECS_HPP
#define ECS_HPP

#include "Base.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "System.hpp"

namespace ecs {

class World {
public:
  //* =============================== Component ===============================

  template <typename ComponentType> //
  void registerComponent()
    requires CValidComponent<ComponentType>
  {
    this->mComponentTable->reg<ComponentType>();
    this->mEntityManager->setNComponents(
        this->mComponentTable->getNComponents());
  }

  template <typename ComponentType> //
  ComponentType &getComponent(u64 entityID)
    requires CValidComponent<ComponentType>
  {
    return this->mComponentTable->get<ComponentType>(entityID);
  }

  template <typename ComponentType, typename EntityType> //
  ComponentType &getComponent(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    return this->getComponent<ComponentType>(entity.getID());
  }

  template <typename ComponentType> //
  void addComponent(u64 entityID, ComponentType component = {})
    requires CValidComponent<ComponentType>
  {
    //* entity must exist
    this->mEntityManager->setSignature(
        entityID, this->mComponentTable->getIndex<ComponentType>(), true);
    this->mComponentTable->add(entityID, std::move(component));
    this->mSystemManager->update(entityID,
                                 this->mEntityManager->getSignature(entityID));
  }

  template <typename ComponentType, typename EntityType> //
  void addComponent(const EntityType &entity, ComponentType component = {})
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->addComponent(entity.getID(), std::move(component));
  }

  template <typename ComponentType> //
  void removeComponent(u64 entityID)
    requires CValidComponent<ComponentType>
  {
    //* entity must exist
    this->mEntityManager->setSignature(
        entityID, this->mComponentTable->getIndex<ComponentType>(), false);
    this->mComponentTable->remove<ComponentType>(entityID);
    this->mSystemManager->update(entityID,
                                 this->mEntityManager->getSignature(entityID));
  }

  template <typename ComponentType, typename EntityType> //
  void removeComponent(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->removeComponent<ComponentType>(entity.getID());
  }

  //* =============================== Entity ==================================

  template <typename EntityType> //
  EntityType getEntity(u64 entityID)
    requires CValidEntity<EntityType>
  {
    return this->mEntityManager->get<EntityType>(entityID);
  }

  template <typename EntityType> //
  EntityType addEntity()
    requires CValidEntity<EntityType>
  {
    return this->mEntityManager->add<EntityType>();
  }

  void removeEntity(u64 entityID) {
    this->mEntityManager->remove(entityID);
    this->mComponentTable->remove(entityID);
    this->mSystemManager->remove(entityID);
  }

  template <typename EntityType> //
  void removeEntity(EntityType &entity)
    requires CValidEntity<EntityType>
  {
    this->removeEntity(entity.getID());
  }

  //* =============================== System ==================================

  template <typename SystemType, typename... RequiredComponentTypes> //
  void registerSystem()
    requires CValidSystem<SystemType>
  {
    this->mSystemManager->reg<SystemType>(std::move(
        this->mComponentTable->querySignature<RequiredComponentTypes...>()));
  }

  template <typename SystemType, typename... Args> //
  void execute(Args &&...args)
    requires CValidSystem<SystemType>
  {
    this->mSystemManager->execute<SystemType>(*this,
                                              std::forward<Args>(args)...);
  }

  //* =============================== Overloading =============================

private:
  template <typename SystemType> //
  const DynamicBitset &getSystemSignature() const
    requires CValidSystem<SystemType>
  {
    return this->mSystemManager->getQualifications<SystemType>();
  }

  template <typename EntityType> //
  const DynamicBitset &getEntitySignature(u64 entityID)
    requires CValidEntity<EntityType>
  {
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

} // namespace ecs

#endif // ECS_HPP