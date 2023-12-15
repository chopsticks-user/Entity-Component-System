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

  //   template <typename Any> //
  //   ComponentType &get(const EntityType &entity)
  //     requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  //   {
  //     return this->get<ComponentType>(entity.getID());
  //   }

private:
};

} // namespace ecs

#endif // ECS_HPP