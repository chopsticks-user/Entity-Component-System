#ifndef ECS_HPP
#define ECS_HPP

#include "Base.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "System.hpp"

namespace ecs {

class World {
public:
  std::unique_ptr<EntityManager> mEntityManager =
      std::make_unique<EntityManager>();
  std::unique_ptr<ComponentTable> mComponentTable =
      std::make_unique<ComponentTable>();
  std::unique_ptr<SystemManager> mSystemManager =
      std::make_unique<SystemManager>();

  //* =============================== Component ===============================

  template <typename ComponentType> //
  void registr()
    requires CValidComponent<ComponentType>
  {
    this->mComponentTable->reg<ComponentType>();
    this->mEntityManager->setNComponents(
        this->mComponentTable->getNComponents());
  }

  template <typename ComponentType> //
  ComponentType &get(u64 entityID)
    requires CValidComponent<ComponentType>
  {
    return this->mComponentTable->get<ComponentType>(entityID);
  }

  template <typename ComponentType, typename EntityType> //
  ComponentType &get(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    return this->get<ComponentType>(entity.getID());
  }

  template <typename ComponentType> //
  void add(u64 entityID, ComponentType component = {})
    requires CValidComponent<ComponentType>
  {
    this->mComponentTable->add(entityID, std::move(component));
  }

  template <typename ComponentType, typename EntityType> //
  void add(const EntityType &entity, ComponentType component = {})
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->add(entity.getID(), std::move(component));
  }

  template <typename ComponentType> //
  void remove(u64 entityID)
    requires CValidComponent<ComponentType>
  {
    this->mComponentTable->remove<ComponentType>(entityID);
  }

  template <typename ComponentType, typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    return this->remove<ComponentType>(entity.getID());
  }

  void remove(u64 entityID) { this->mComponentTable->remove(entityID); }

  template <typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidEntity<EntityType>
  {
    return this->remove(entity.getID());
  }

  //* =============================== Entity ==================================

  template <typename EntityType> //
  EntityType get(u64 entityID)
    requires CValidEntity<EntityType>
  {
    return this->mEntityManager->get<EntityType>(entityID);
  }

  template <typename EntityType> //
  EntityType add()
    requires CValidEntity<EntityType>
  {
    return this->mEntityManager->add<EntityType>();
  }

  template <typename EntityType> //
  void remove(u64 entityID)
    requires CValidEntity<EntityType>
  {
    this->mEntityManager->remove(entityID);
  }

  template <typename EntityType> //
  void remove(EntityType &entity)
    requires CValidEntity<EntityType>
  {
    this->mEntityManager->remove(entity);
  }

private:
  template <typename EntityType> //
  const DynamicBitset &getSignature(u64 entityID)
    requires CValidEntity<EntityType>
  {
    return this->mEntityManager->getSignature(entityID);
  }

  //* =============================== System ==================================

public:
  template <typename SystemType, typename... RequiredComponentTypes> //
  void registr()
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

  template <typename SystemType> //
  void add(u64 entityID)
    requires CValidSystem<SystemType>
  {
    //* Biset or
    auto qualifications = this->getSignature<SystemType>();

    //! A copy of DynamicBitset
    DynamicBitset entitySignature =
        this->mEntityManager->getSignature(entityID);
    for (u64 i = 0; i < qualifications.size(); ++i) {
      if (qualifications[i] == true && entitySignature[i] == false) {
        entitySignature[i] = true;
      }
    }
    this->mEntityManager->setSignature(entityID, std::move(entitySignature));

    return this->mSystemManager->add<SystemType>(
        entityID, this->mEntityManager->getSignature(entityID));
  }

  template <typename SystemType, typename EntityType> //
  void add(const EntityType &entity)
    requires CValidSystem<SystemType> && CValidEntity<EntityType>
  {
    return this->add<SystemType>(entity.getID());
  }

  template <typename SystemType> //
  void remove(u64 entityID)
    requires CValidSystem<SystemType>
  {
    this->mSystemManager->remove(entityID);
  }

  template <typename SystemType> //
  void remove(SystemType &entity)
    requires CValidSystem<SystemType>
  {
    this->remove(entity.getID());
  }

private:
  template <typename SystemType> //
  const DynamicBitset &getSignature() const
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
  // std::unique_ptr<EntityManager> mEntityManager =
  //     std::make_unique<EntityManager>();
  // std::unique_ptr<ComponentTable> mComponentTable =
  //     std::make_unique<ComponentTable>();
  // std::unique_ptr<SystemManager> mSystemManager =
  //     std::make_unique<SystemManager>();
};

} // namespace ecs

#endif // ECS_HPP