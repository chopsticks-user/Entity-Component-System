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

protected:
private:
  Entity(u64 id) : mID{id} {}

private:
  // const World &mRefWorld;
  u64 mID;
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
  EntityType get(u64 entityID) const
    requires CValidEntity<EntityType>
  {
    try {
      return EntityType{entityID};
    } catch (std::out_of_range &e) {
      throw std::runtime_error("EntityManager::get: unknown entity");
    }
  }

  template <typename EntityType> //
  EntityType add()
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
    return EntityType{newEntityId};
  }

  void remove(u64 entityID) {
    this->mEntityInfos.erase(entityID);
    // this->mRecycledIDs.push(entityBase.mID);
  }

  template <typename EntityType> //
  void remove(EntityType &entity)
    requires CValidEntity<EntityType>
  {
    Entity *entityBase = &entity;

    if (entityBase->mID == 0) {
      return;
    }

    // this->mRecycledIDs.push(entityBase.mID);
    entityBase->mID = 0;

    this->remove(entityBase->getID());
  }

  const DynamicBitset &getSignature(u64 entityID) const {
    try {
      return this->mEntityInfos.at(entityID);
    } catch (std::out_of_range &e) {
      throw std::runtime_error("EntityManager::getSignature: unknown entity");
    }
  }

  void setSignature(u64 entityID, u64 index, bool value = true) {
    if (index >= this->mNComponents) {
      throw std::runtime_error(
          "EntityManager::setSignature: index out of range");
    }

    try {
      DynamicBitset &refSignature = this->mEntityInfos.at(entityID);
      if (refSignature.size() != this->mNComponents) {
        refSignature.resize(this->mNComponents);
      }
      refSignature[index] = value;
    } catch (std::out_of_range &e) {
      throw std::runtime_error("EntityManager::setSignature: unknown entity");
    }
  }

  // template <typename EntityType> //
  // void setSignature(EntityType &entity, u64 index, bool value = true) const
  //   requires CValidEntity<EntityType>
  // {
  //   if (!this->isKnownEntity(entity)) {
  //     throw std::runtime_error("EntityManager::setSignature: unknown
  //     entity");
  //   }

  //   this->setSignature(entity.getID(), index, value);
  //   const DynamicBitset &signature = this->mEntityInfos[entity.getID()];
  //   std::copy(signature.begin(), signature.end(), entity.mSignature.begin());
  //   // std::copy(entity.mSignature, this->mEntityInfos[entity.getID()]);
  // }

  void setSignature(u64 entityID, DynamicBitset newSignature) {
    // if (!this->isKnownEntity(entity)) {
    //   throw std::runtime_error("EntityManager::setSignature: unknown
    //   entity");
    // }

    if (newSignature.size() != this->mNComponents) {
      throw std::runtime_error(
          "EntityManager::setSignature: incorrect nComponents");
    }

    try {
      this->mEntityInfos.at(entityID) = std::move(newSignature);
    } catch (std::out_of_range &e) {
      throw std::runtime_error("EntityManager::setSignature: unknown entity");
    }
  }

  // template <typename EntityType> //
  // void setSignature(EntityType &entity, DynamicBitset newSignature) const
  //   requires CValidEntity<EntityType>
  // {
  //   this->setSignature(entity.getID(), std::move(newSignature));
  // }

private:
  // const World &mRefWorld;
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