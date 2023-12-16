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
  void addComponentToEntity(u64 entityID, ComponentType component = {})
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
  void addComponentToEntity(const EntityType &entity,
                            ComponentType component = {})
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->addComponentToEntity(entity.getID(), std::move(component));
  }

  template <typename ComponentType> //
  void removeComponentFromEntity(u64 entityID)
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
  void removeComponentFromEntity(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->removeComponentFromEntity<ComponentType>(entity.getID());
  }

  // void removeComponentFromEntity(u64 entityID) {
  //   //* entity must exist
  //   this->mEntityManager->setSignature(entityID, {});
  //   this->mComponentTable->remove(entityID);
  // }

  // template <typename EntityType> //
  // void removeComponentFromEntity(const EntityType &entity)
  //   requires CValidEntity<EntityType>
  // {
  //   this->removeComponentFromEntity(entity.getID());
  // }

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

private:
  template <typename EntityType> //
  const DynamicBitset &getEntitySignature(u64 entityID)
    requires CValidEntity<EntityType>
  {
    return this->mEntityManager->getSignature(entityID);
  }

  //* =============================== System ==================================

public:
  template <typename SystemType, typename... RequiredComponentTypes> //
  void registerSystem()
    requires CValidSystem<SystemType>
  {
    this->mSystemManager->reg<SystemType>(std::move(
        this->mComponentTable->querySignature<RequiredComponentTypes...>()));
  }

  // template <typename SystemType> //
  // SystemType &get()
  //   requires CValidSystem<SystemType>
  // {
  //   return *(this->mSystemManager->get<SystemType>());
  // }

  // //* Make the entity qualified for the system
  // template <typename SystemType> //
  // void addEntityToSystem(u64 entityID)
  //   requires CValidSystem<SystemType>
  // {
  //   auto qualifications = this->getSystemSignature<SystemType>();

  //   //! A copy of DynamicBitset
  //   DynamicBitset entitySignature =
  //       this->mEntityManager->getSignature(entityID);
  //   for (u64 i = 0; i < qualifications.size(); ++i) {
  //     if (qualifications[i] == true && entitySignature[i] == false) {
  //       entitySignature[i] = true;
  //     }
  //   }
  //   this->mEntityManager->setSignature(entityID, std::move(entitySignature));

  //   return this->mSystemManager->add<SystemType>(
  //       entityID, this->mEntityManager->getSignature(entityID));
  // }

  // template <typename SystemType, typename EntityType> //
  // void addEntityToSystem(const EntityType &entity)
  //   requires CValidSystem<SystemType> && CValidEntity<EntityType>
  // {
  //   return this->addEntityToSystem<SystemType>(entity.getID());
  // }

  // template <typename SystemType> //
  // void removeEntityFromSystem(u64 entityID)
  //   requires CValidSystem<SystemType>
  // {
  //   this->mSystemManager->remove(entityID);
  // }

  // template <typename SystemType, typename EntityType> //
  // void removeEntityFromSystem(EntityType &entity)
  //   requires CValidSystem<SystemType>
  // {
  //   this->removeEntityFromSystem(entity.getID());
  // }

  // void removeEntityFromSystem(u64 entityID) {
  //   this->mSystemManager->remove(entityID);
  // }

  // void removeEntityFromSystem(SystemType &entity) {
  //   this->removeEntityFromSystem(entity.getID());
  // }

private:
  template <typename SystemType> //
  const DynamicBitset &getSystemSignature() const
    requires CValidSystem<SystemType>
  {
    return this->mSystemManager->getQualifications<SystemType>();
  }

  // template <typename SystemType> //
  // const DynamicBitset &setSignature() const
  //   requires CValidSystem<SystemType>
  // {
  //   return this->mSystemManager->getQualifications<SystemType>();
  // }

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