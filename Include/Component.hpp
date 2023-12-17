#ifndef ECS_COMPONENT_HPP
#define ECS_COMPONENT_HPP

#include "Base.hpp"
#include "Entity.hpp"

namespace ecs {

struct Component {};

template <typename ComponentType>
concept CValidComponent = std::copy_constructible<ComponentType> &&
                          std::derived_from<ComponentType, Component> &&
                          !std::is_same_v<ComponentType, Component>;

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
    //* Ignore the second registration
    auto result = this->mComponentData.try_emplace(
        typenameStr<ComponentType>(),
        std::make_shared<SparseVector<ComponentType>>());
    if (result.second == true) {
      this->mCNameToIndex[typenameStr<ComponentType>()] =
          this->mCNameToIndex.size();
    }
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

  template <typename ComponentType> //
  void add(u64 entityID, ComponentType component = {})
    requires CValidComponent<ComponentType>
  {
    this->getArray<ComponentType>()->add(entityID, std::move(component));
  }

  template <typename ComponentType, typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->getArray<ComponentType>()->remove(entity.getID());
  }

  template <typename ComponentType> //
  void remove(u64 entityID)
    requires CValidComponent<ComponentType>
  {
    this->getArray<ComponentType>()->remove(entityID);
  }

  template <typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidEntity<EntityType>
  {
    for (auto &p : this->mComponentData) {
      p.second->remove(entity.getID());
    }
  }

  void remove(u64 entityID) {
    for (auto &p : this->mComponentData) {
      p.second->remove(entityID);
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
    try {
      return this->getArray<ComponentType>()->at(entity.getID());
    } catch (std::out_of_range &e) {
      std::runtime_error("ComponentTable::get: entity does not exist or does "
                         "not contain the component");
    }
  }

  template <typename ComponentType> //
  ComponentType &get(u64 entityID)
    requires CValidComponent<ComponentType>
  {
    return this->getArray<ComponentType>()->at(entityID);
  }

  template <typename ComponentType> //
  u64 getIndex()
    requires CValidComponent<ComponentType>
  {
    try {
      return this->mCNameToIndex.at(typenameStr<ComponentType>());
    } catch (std::out_of_range &e) {
      throw std::runtime_error(
          "ComponentTable::getIndex: unregistered component");
    }
  }

  template <typename... ComponentTypes> //
  DynamicBitset querySignature() {
    DynamicBitset signature(this->mCNameToIndex.size());
    auto argTuple = std::tuple<ComponentTypes...>();
    iterateTuple<0, ComponentTypes...>(argTuple, [&](auto arg) {
      signature[this->getIndex<decltype(arg)>()] = true;
    });
    return signature;
  }

  void clear() noexcept {
    this->mComponentData.clear();
    this->mCNameToIndex.clear();
  }

private:
  std::unordered_map<cString, u64> mCNameToIndex = {};
  std::unordered_map<cString, std::shared_ptr<ISparseVector>> mComponentData =
      {};
};

} // namespace ecs

#endif // ECS_COMPONENT_HPP