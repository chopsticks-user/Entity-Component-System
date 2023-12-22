#ifndef ECS_SOURCE_ENTITY_HPP
#define ECS_SOURCE_ENTITY_HPP

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

class EntityManager {
public:
  EntityManager() = default;
  ~EntityManager() noexcept = default;
  EntityManager(const EntityManager &) = delete;
  EntityManager(EntityManager &&) noexcept = default;
  EntityManager &operator=(const EntityManager &) = delete;
  EntityManager &operator=(EntityManager &&) noexcept = default;

  bool exists(const u64 entityID) const noexcept {
    return this->mEntityInfos.contains(entityID);
  }

  u64 nEntities() const noexcept { return this->mEntityInfos.size(); }

  void setNComponents(u64 nComponents) noexcept {
    this->mNComponents = nComponents;
  }

  /**
   * @throws std::out_of_range if entityID is not registered.
   */
  template <typename EntityType> //
  EntityType get(u64 entityID) const noexcept(false)
    requires CValidEntity<EntityType>
  {
    if (!this->exists(entityID)) {
      throw std::out_of_range("EntityManager::get: unknown entity");
    }
    return EntityType{entityID};
  }

  /**
   * @throws std::bad_alloc from std::unordered_map.
   * @throws
   * TODO: check if this->mCurrentID is overflow
   */
  template <typename EntityType> //
  EntityType add() noexcept(false)
    requires CValidEntity<EntityType>
  {
    u64 newEntityId;
    newEntityId = this->mCurrentID++;
    this->mEntityInfos[newEntityId] = {};
    return EntityType{newEntityId};
  }

  void remove(u64 entityID) noexcept { this->mEntityInfos.erase(entityID); }

  template <typename EntityType> //
  void remove(EntityType &entity) noexcept
    requires CValidEntity<EntityType>
  {
    Entity *entityBase = &entity;

    if (entityBase->mID == 0) {
      return;
    }

    entityBase->mID = 0;

    this->remove(entityBase->getID());
  }

  /**
   * @throws std::out_of_range from std::unordered_map.
   */
  const DynamicBitset &getSignature(u64 entityID) const noexcept(false) {
    return this->mEntityInfos.at(entityID);
  }

  /**
   * @throws std::out_of_range from std::unordered_map.
   */
  void setSignature(u64 entityID, u64 index, bool value = true) {
    DynamicBitset &refSignature = this->mEntityInfos.at(entityID);
    if (refSignature.size() != this->mNComponents) {
      refSignature.resize(this->mNComponents);
    }
    refSignature.set(index, value);
  }

  /**
   * @throws std::out_of_range from std::unordered_map.
   */
  void setSignature(u64 entityID, DynamicBitset newSignature) {
    this->mEntityInfos.at(entityID) = std::move(newSignature);
  }

  void clear() noexcept {
    this->mEntityInfos.clear();
    this->mCurrentID = 1;
    this->mNComponents = 0;
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

#endif // ECS_SOURCE_ENTITY_HPP