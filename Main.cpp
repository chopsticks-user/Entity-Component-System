#include <glm/glm.hpp>

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <concepts>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <vector>

typedef uint64_t u64;
typedef double f64;
typedef const char *cString;

class EntityManager;
class World;

class Entity {
  friend class EntityManager;

public:
  Entity() = delete;
  virtual ~Entity() = default;
  Entity(const Entity &) = delete;
  Entity(Entity &&) = default;
  Entity &operator=(const Entity &) = delete;
  Entity &operator=(Entity &&) = default;

  virtual u64 getID() const noexcept final { return this->mID; }
  virtual const std::vector<bool> &getSignature() const noexcept final {
    return this->mSignature;
  }

protected:
  Entity(const World &world, u64 id, std::vector<bool> signature = {})
      : mRefWorld{world}, mID{id}, mSignature{std::move(signature)} {}

private:
  const World &mRefWorld;
  u64 mID;
  std::vector<bool> mSignature;
};

class Component {
public:
protected:
  Component() = default;
};

class System {};

void runtimeAssert(bool condition, const char *exceptionMessage) {
  if (!condition) {
    throw std::runtime_error(exceptionMessage);
  }
}

template <typename EntityType>
concept CValidEntity = std::derived_from<EntityType, Entity> &&
                       !std::is_same_v<EntityType, Entity>;

template <typename ComponentType>
concept CValidComponent = std::derived_from<ComponentType, Component> &&
                          !std::is_same_v<ComponentType, Component>;

template <typename Type> //
cString typenameStr() noexcept {
  return typeid(Type).name();
}

class EntityManager final {
public:
  EntityManager() = delete;
  ~EntityManager() = default;
  explicit EntityManager(const World &world) : mRefWorld{world} {}
  EntityManager(const EntityManager &) = delete;
  EntityManager(EntityManager &&) = default;
  EntityManager &operator=(const EntityManager &) = delete;
  EntityManager &operator=(EntityManager &&) = default;

  bool isFull() const noexcept {
    return this->mCurrentID > std::numeric_limits<u64>::max();
  }

  template <typename EntityType> //
  bool isKnownEntity(const EntityType &entity) const noexcept
    requires CValidEntity<EntityType>
  {
    const Entity *entityBase = &entity;
    return entityBase->mID != 0 && std::addressof(entityBase->mRefWorld) ==
                                       std::addressof(this->mRefWorld);
  }

  void setNComponents(u64 nComponents) noexcept {
    this->mNComponents = nComponents;
  }

  template <typename EntityType> //
  EntityType createEntity()
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
    return EntityType(this->mRefWorld, newEntityId,
                      std::vector<bool>(this->mNComponents));
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
  void setSignature(EntityType &entity, std::vector<bool> newSignature) const
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
  const World &mRefWorld;
  u64 mCurrentID = 1;
  u64 mNComponents = 0;
  // std::queue<u64> mRecycledIDs = {};
};

class ISparseVector {
public:
  virtual ~ISparseVector() = default;
  virtual void remove(u64) = 0;
};

// template <typename DataType, typename IDType = u64> //
template <typename DataType> //
class SparseVector : public ISparseVector {
public:
  u64 size() const noexcept { return this->mData.size(); }

  auto begin() noexcept { return this->mData.begin(); }

  auto end() noexcept { return this->mData.end(); }

  DataType &at(u64 id) {
    if (!this->exists(id)) {
      throw std::runtime_error("SparseVector::operator[]: unknown ID");
    }
    return this->mData[this->mIDToIndex[id]];
  }

  DataType &operator[](u64 id) { return this->at(id); }

  DataType &operator[](u64 id) const {
    if (!this->exists(id)) {
      throw std::runtime_error("SparseVector::operator[]: unknown ID");
    }
    auto index = this->mIDToIndex.at(id);
    return this->mData[index];
  }

  void add(u64 id, DataType value) {
    if (!this->exists(id)) {
      this->mData.emplace_back(value);
      this->mIDToIndex[id] = this->mData.size() - 1;
      this->mIndexToID[this->mData.size() - 1] = id;
    } else {
      this->mData[this->mIDToIndex[id]] = std::move(value);
    }
  }

  void remove(u64 id) override {
    if (!this->exists(id)) {
      return;
    }

    auto lastElementIndex = this->mData.size() - 1;
    auto lastElementID = this->mIndexToID[lastElementIndex];
    auto removedIndex = this->mIDToIndex[id];

    this->mIndexToID[removedIndex] = lastElementID;
    this->mIDToIndex[lastElementID] = removedIndex;

    std::swap(this->mData[removedIndex], this->mData[lastElementIndex]);
    this->mIndexToID.erase(lastElementIndex);
    this->mIDToIndex.erase(id);
    this->mData.pop_back();
  }

private:
  std::unordered_map<u64, u64> mIDToIndex = {};
  std::unordered_map<u64, u64> mIndexToID = {};
  std::vector<DataType> mData = {};

  bool exists(const u64 &id) {
    return this->mIDToIndex.find(id) != this->mIDToIndex.end();
  }
};

class ComponentTable final {
public:
  ComponentTable() = default;
  ~ComponentTable() = default;
  ComponentTable(const ComponentTable &) = delete;
  ComponentTable(ComponentTable &&) = default;
  ComponentTable &operator=(const ComponentTable &) = delete;
  ComponentTable &operator=(ComponentTable &&) = default;

  u64 getNComponents() const noexcept { return this->mComponentData.size(); }

  template <typename ComponentType> //
  void reg()
    requires CValidComponent<ComponentType>
  {
    this->mComponentData.try_emplace(
        typenameStr<ComponentType>(),
        std::make_shared<SparseVector<ComponentType>>());
  }

  // template <typename ComponentType> //
  // ComponentTable &dereg()
  //   requires CValidComponent<ComponentType>
  // {
  //   componentTable.erase(typenameStr<ComponentType>());
  // }

  template <typename ComponentType, typename EntityType> //
  void add(const EntityType &entity, ComponentType component = {})
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->getArray<ComponentType>()->add(entity.getID(), std::move(component));
  }

  template <typename ComponentType, typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->getArray<ComponentType>()->remove(entity.getID());
  }

  template <typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidEntity<EntityType>
  {
    for (auto &pair : this->mComponentData) {
      pair.second->remove(entity.getID());
    }
  }

  template <typename ComponentType> //
  std::shared_ptr<SparseVector<ComponentType>> getArray()
    requires CValidComponent<ComponentType>
  {
    try {
      return std::static_pointer_cast<SparseVector<ComponentType>>(
          this->mComponentData.at(typenameStr<ComponentType>()));
    } catch (std::out_of_range &e) {
      throw std::runtime_error(
          "ComponentTable::getArray: unregistered component");
    }
  }

  template <typename ComponentType, typename EntityType> //
  ComponentType &get(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    return this->getArray<ComponentType>()->at(entity.getID());
  }

  void clear() { this->mComponentData.clear(); }

private:
  std::unordered_map<cString, std::shared_ptr<ISparseVector>> mComponentData =
      {};
};

class SystemManager {};

class World {
public:
private:
  std::unique_ptr<EntityManager> mEntityManager =
      std::make_unique<EntityManager>(*this);
  std::unique_ptr<ComponentTable> mComponentTable =
      std::make_unique<ComponentTable>();
  std::unique_ptr<SystemManager> mSystemManager =
      std::make_unique<SystemManager>();
};

class Motion : public Component {};
class Mesh : public Component {};

class NPC : public Entity {
  using Entity::Entity;

public:
  int n = 5;
};

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  World w;
  ComponentTable t;
  EntityManager m(w);

  t.reg<Motion>();
  t.reg<Mesh>();

  m.setNComponents(t.getNComponents());
  NPC e = m.createEntity<NPC>();
  t.add<Motion>(e);
  std::cout << std::boolalpha << m.isKnownEntity(e) << std::endl;
  std::cout << e.n << std::endl;
  m.setSignature(e, 1, true);
  m.destroyEntity(e);
  std::cout << std::boolalpha << m.isKnownEntity(e) << std::endl;
  return EXIT_SUCCESS;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    return protected_main(argc, argv);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Uncaught exception" << std::endl;
  }
  return EXIT_FAILURE;
}