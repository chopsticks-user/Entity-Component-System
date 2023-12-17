#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP

#include "Base.hpp"

namespace ecs {

class EntityManager;
class World;

class Entity {
  friend class EntityManager;

public:
  Entity() noexcept = delete;
  virtual ~Entity() = default;
  Entity(const Entity &) = delete;
  Entity(Entity &&) noexcept = default;
  Entity &operator=(const Entity &) = delete;
  Entity &operator=(Entity &&) noexcept = default;

  virtual u64 getID() const noexcept final { return this->mID; }

protected:
private:
  Entity(u64 id) : mID{id} {}

private:
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
  EntityManager(EntityManager &&) noexcept = default;
  EntityManager &operator=(const EntityManager &) = delete;
  EntityManager &operator=(EntityManager &&) noexcept = default;

  void setNComponents(u64 nComponents) noexcept {
    this->mNComponents = nComponents;
  }

  template <typename EntityType> //
  EntityType get(u64 entityID) const
    requires CValidEntity<EntityType>
  {
    expect(this->exists(entityID), "EntityManager::get: unknown entity");
    return EntityType{entityID};
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

  void remove(u64 entityID) noexcept {
    this->mEntityInfos.erase(entityID);
    // this->mRecycledIDs.push(entityBase.mID);
  }

  template <typename EntityType> //
  void remove(EntityType &entity) noexcept
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
    //! Debug
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

  void setSignature(u64 entityID, DynamicBitset newSignature) {

    //! Debug
    expect(newSignature.size() == this->mNComponents,
           "EntityManager::setSignature: incorrect nComponents");

    try {
      this->mEntityInfos.at(entityID) = std::move(newSignature);
    } catch (std::out_of_range &e) {
      throw std::runtime_error("EntityManager::setSignature: unknown entity");
    }
  }

  void clear() noexcept {
    this->mEntityInfos.clear();
    this->mCurrentID = 1;
    this->mNComponents = 0;
  }

private:
  bool exists(const u64 entityID) const noexcept {
    return this->mEntityInfos.find(entityID) != this->mEntityInfos.end();
  }

private:
  // const World &mRefWorld;
  std::unordered_map<u64, DynamicBitset> mEntityInfos;
  u64 mCurrentID = 1;
  u64 mNComponents = 0;
  // std::queue<u64> mRecycledIDs = {};
};

#define ECS_SIMPLE_ENTITY_CLASS(EntityTypename)                                \
  class EntityTypename : public ecs::Entity {                                  \
    using Entity::Entity;                                                      \
  }

} // namespace ecs

#endif // ECS_ENTITY_HPP