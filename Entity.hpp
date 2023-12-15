#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP

#include "Base.hpp"

namespace ecs {

class EntityManager;
class World;

class Entity {
  friend class EntityManager;

public:
  Entity() = delete;
  virtual ~Entity() = default;
  Entity(const Entity &) = delete;
  Entity &operator=(const Entity &) = delete;
  Entity(Entity &&) = default;
  Entity &operator=(Entity &&) = default;

  virtual u64 getID() const noexcept final { return this->mID; }

  virtual const DynamicBitset &getSignature() const noexcept final {
    return this->mSignature;
  }

protected:
private:
  Entity(const World &world, u64 id, DynamicBitset signature = {})
      : mRefWorld{world}, mID{id}, mSignature{std::move(signature)} {}

private:
  const World &mRefWorld;
  u64 mID;
  DynamicBitset mSignature;
};

template <typename EntityType>
concept CValidEntity = std::move_constructible<EntityType> &&
                       std::derived_from<EntityType, Entity> &&
                       !std::is_same_v<EntityType, Entity>;

class EntityManager final {
public:
  EntityManager() = default;
  ~EntityManager() = default;
  EntityManager(const EntityManager &) = delete;
  EntityManager(EntityManager &&) = default;
  EntityManager &operator=(const EntityManager &) = delete;
  EntityManager &operator=(EntityManager &&) = default;

  bool isFull() const noexcept {
    return this->mCurrentID > std::numeric_limits<u64>::max();
  }

  // template <typename EntityType> //
  // bool isKnownEntity(const EntityType &entity) const noexcept
  //   requires CValidEntity<EntityType>
  // {
  //   const Entity *entityBase = &entity;
  //   return entityBase->mID != 0 && std::addressof(entityBase->mRefWorld) ==
  //                                      std::addressof(this->mRefWorld);
  // }

  void setNComponents(u64 nComponents) noexcept {
    this->mNComponents = nComponents;
  }

  template <typename EntityType> //
  EntityType createEntity(const World &world)
    requires CValidEntity<EntityType>
  {
    u64 newEntityId;
    // if (!mRecycledIDs.empty()) {
    //   newEntityId = this->mRecycledIDs.front();
    //   this->mRecycledIDs.pop();
    // } else {
    //   newEntityId = this->mCurrentID++;
    // }
    newEntityId = this->mCurrentID++;
    this->mEntityInfos[newEntityId] = {};
    return EntityType{world, newEntityId, {}};
  }

  template <typename EntityType> //
  void destroyEntity(EntityType &entity)
    requires CValidEntity<EntityType>
  {
    Entity *entityBase = &entity;

    if (entityBase->mID == 0) {
      return;
    }

    // this->mRecycledIDs.push(entityBase.mID);
    entityBase->mID = 0;
    entityBase->mSignature.clear();
  }

  template <typename EntityType> //
  void setSignature(EntityType &entity, u64 index, bool value = true) const
    requires CValidEntity<EntityType>
  {
    if (index >= this->mNComponents) {
      throw std::runtime_error(
          "EntityManager::setSignature: index out of range");
    }

    Entity *entityBase = &entity;
    if (entityBase->mSignature.size() != this->mNComponents) {
      entityBase->mSignature.resize(this->mNComponents);
    }
    entityBase->mSignature[index] = value;
  }

  template <typename EntityType> //
  void setSignature(EntityType &entity, DynamicBitset newSignature) const
    requires CValidEntity<EntityType>
  {
    if (newSignature.size() != this->mNComponents) {
      throw std::runtime_error(
          "EntityManager::setSignature: incorrect nComponents");
    }

    Entity *entityBase = &entity;
    entityBase->mSignature = std::move(newSignature);
  }

private:
  std::unordered_map<u64, DynamicBitset> mEntityInfos;
  u64 mCurrentID = 1;
  u64 mNComponents = 0;
  // std::queue<u64> mRecycledIDs = {};
};

#define DECLARE_SIMPLE_ENTITY(EntityTypename)                                  \
  class EntityTypename : public ecs::Entity {                                  \
    using Entity::Entity;                                                      \
  }

} // namespace ecs

#endif // ECS_ENTITY_HPP