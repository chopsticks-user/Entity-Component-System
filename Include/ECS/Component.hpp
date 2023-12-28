#ifndef ECS_INCLUDE_ECS_COMPONENT_HPP
#define ECS_INCLUDE_ECS_COMPONENT_HPP

#include "Base.hpp"
#include "Entity.hpp"

namespace ecs {

struct Component {};

class ComponentTable {
public:
  ComponentTable() = default;
  ~ComponentTable() = default;
  ComponentTable(const ComponentTable &) = delete;
  ComponentTable(ComponentTable &&) noexcept = default;
  ComponentTable &operator=(const ComponentTable &) = delete;
  ComponentTable &operator=(ComponentTable &&) noexcept = default;

  u64 getNComponents() const noexcept { return this->mComponentData.size(); }

  /**
   * @brief Ignore the second registration.
   * @throws std::bad_alloc from either std::shared_ptr or std::unordered_map.
   */
  template <typename ComponentType> //
  void reg() noexcept(false)
    requires CValidComponent<ComponentType>
  {
    auto result = this->mComponentData.try_emplace(
        typenameStr<ComponentType>(),
        std::make_shared<SparseVector<ComponentType>>());
    if (result.second == true) {
      this->mCNameToIndex[typenameStr<ComponentType>()] =
          this->mCNameToIndex.size();
    }
  }

  /**
   * @throws std::bad_alloc from container::SparseVector
   */
  template <typename ComponentType, typename EntityType> //
  void add(const EntityType &entity,
           ComponentType component = {}) noexcept(false)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->getArray<ComponentType>()->add(entity.getID(), std::move(component));
  }

  /**
   * @throws std::bad_alloc from container::SparseVector
   */
  template <typename ComponentType> //
  void add(u64 entityID, ComponentType component = {}) noexcept(false)
    requires CValidComponent<ComponentType>
  {
    this->getArray<ComponentType>()->add(entityID, std::move(component));
  }

  template <typename ComponentType, typename EntityType> //
  void remove(const EntityType &entity) noexcept
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->getArray<ComponentType>()->remove(entity.getID());
  }

  template <typename ComponentType> //
  void remove(u64 entityID) noexcept
    requires CValidComponent<ComponentType>
  {
    this->getArray<ComponentType>()->remove(entityID);
  }

  template <typename EntityType> //
  void remove(const EntityType &entity) noexcept
    requires CValidEntity<EntityType>
  {
    for (auto &p : this->mComponentData) {
      p.second->remove(entity.getID());
    }
  }

  void remove(u64 entityID) noexcept {
    for (auto &p : this->mComponentData) {
      p.second->remove(entityID);
    }
  }

  template <typename ComponentType, typename EntityType> //
  ComponentType &get(const EntityType &entity) noexcept(false)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    return this->getArray<ComponentType>()->at(entity.getID());
  }

  /**
   * @throws std::out_of_range from std::unordered_map
   */
  template <typename ComponentType> //
  ComponentType &get(u64 entityID) noexcept(false)
    requires CValidComponent<ComponentType>
  {
    return this->getArray<ComponentType>()->at(entityID);
  }

  /**
   * @throws std::out_of_range from std::unordered_map
   */
  template <typename ComponentType> //
  u64 getIndex() const noexcept(false)
    requires CValidComponent<ComponentType>
  {
    return this->mCNameToIndex.at(typenameStr<ComponentType>());
  }

  /**
   * @throws std::out_of_range from ecs::Component, std::bad_alloc from
   * container::DynamicBitset.
   */
  template <typename... ComponentTypes> //
  DynamicBitset querySignature() const noexcept(false) {
    DynamicBitset signature(this->mCNameToIndex.size());
    auto argTuple = std::tuple<ComponentTypes...>();
    iterateTuple<0, ComponentTypes...>(argTuple, [&](auto arg) {
      // signature[this->getIndex<decltype(arg)>()] = true;
      signature.set(this->getIndex<decltype(arg)>());
    });
    return signature;
  }

  void clear() noexcept {
    this->mComponentData.clear();
    this->mCNameToIndex.clear();
  }

private:
  /**
   * @throws std::out_of_range from std::unordered_map.
   */
  template <typename ComponentType> //
  std::shared_ptr<SparseVector<ComponentType>> getArray() noexcept(false)
    requires CValidComponent<ComponentType>
  {
    return std::static_pointer_cast<SparseVector<ComponentType>>(
        this->mComponentData.at(typenameStr<ComponentType>()));
  }

private:
  std::unordered_map<cString, u64> mCNameToIndex = {};
  std::unordered_map<cString, std::shared_ptr<ISparseVector>> mComponentData =
      {};
};

} // namespace ecs

#endif // ECS_INCLUDE_ECS_COMPONENT_HPP