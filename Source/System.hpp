#ifndef ECS_SOURCE_SYSTEM_HPP
#define ECS_SOURCE_SYSTEM_HPP

#include "Base.hpp"
#include "Entity.hpp"

namespace ecs {

class World;

class System {
  friend class SystemManager;

public:
  virtual ~System() = default;
  System(const System &) = delete;
  System(System &&) noexcept = default;
  System &operator=(const System &) = delete;
  System &operator=(System &&) noexcept = default;

protected:
  System() = default;

private:
  // TODO: Move all data members to SystemManager
  UniqueIDContainer mEntityIDs = {};
  DynamicBitset mQualifications = {};
};

template <typename SystemType>
concept CValidSystem = std::move_constructible<SystemType> &&
                       std::derived_from<SystemType, System> &&
                       !std::is_same_v<SystemType, System>;

template <typename FunctionType>
concept CValidSystemFunction =
    std::is_same_v<typename First2ArgTypes<FunctionType>::type1, World &> &&
    std::is_same_v<typename First2ArgTypes<FunctionType>::type2,
                   const UniqueIDContainer &>;

class SystemManager final {
public:
  SystemManager() = default;
  ~SystemManager() = default;
  SystemManager(const SystemManager &) = delete;
  SystemManager(SystemManager &&) noexcept = default;
  SystemManager &operator=(const SystemManager &) = delete;
  SystemManager &operator=(SystemManager &&) noexcept = default;

  u64 getNSystems() const noexcept { return this->mSystems.size(); }

  template <typename SystemType> //
  void reg(DynamicBitset qualifications = {})
    requires CValidSystem<SystemType>
  {
    auto typeStr = typenameStr<SystemType>();

    //* System must be static, that is, the qualified condition does not change.
    //* Also, a system cannot be registered more than once.
    if (this->mSystems.find(typeStr) != this->mSystems.end()) {
      return; // already registered
    }

    System *pSystemBase = new SystemType();
    pSystemBase->mQualifications = std::move(qualifications);
    this->mSystems[typenameStr<SystemType>()] =
        std::unique_ptr<System>(std::move(pSystemBase));
  }

  // template <typename SystemType> //
  // void dereg()
  //   requires CValidSystem<SystemType>
  // {
  //   this->mSystems.erase(typenameStr<SystemType>());
  // }

  template <typename SystemType> //
  std::unique_ptr<SystemType> get() const
    requires CValidSystem<SystemType>
  {
    try {
      return std::static_pointer_cast<SystemType>(
          this->mSystems.at(typenameStr<SystemType>()));
    } catch (std::out_of_range &e) {
      throw std::runtime_error("SystemManaged::get: unregistered system");
    }
  }

  template <typename SystemType> //
  const DynamicBitset &getQualifications() const
    requires CValidSystem<SystemType>
  {
    return this->get<SystemType>()->mQualifications;
  }

  template <typename SystemType> //
  const DynamicBitset &setQualifications(DynamicBitset qualifications) const
    requires CValidSystem<SystemType>
  {
    return this->get<SystemType>()->mQualifications = std::move(qualifications);
  }

  template <typename SystemType> //
  void add(u64 entityID, const DynamicBitset &entitySignature)
    requires CValidSystem<SystemType>
  {
    if (!std::equal(entitySignature.begin(), entitySignature.end(),
                    this->getQualifications<SystemType>().begin())) {
      throw std::runtime_error("SystemManaged::add: unqualified entity");
    }
  }

  template <typename SystemType> //
  void remove(u64 entityID)
    requires CValidSystem<SystemType>
  {
    this->get<SystemType>()->mQualifiedEntitiesIDs.remove(entityID);
  }

  template <typename SystemType, typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidSystem<SystemType> && CValidEntity<EntityType>
  {
    this->remove<SystemType>(entity.getID());
  }

  void remove(u64 entityID) {
    for (auto &p : this->mSystems) {
      // p.second->mEntityIDs.erase(entityID);
      p.second->mEntityIDs.remove(entityID);
    }
  }

  template <typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidEntity<EntityType>
  {
    this->remove(entity.getID());
  }

  void update(u64 entityID, const DynamicBitset &newEntitySignature) {
    for (auto &p : this->mSystems) {
      const auto &requiredSignature = p.second->mQualifications;

      u64 i = 0;
      for (; i < requiredSignature.size(); ++i) {
        if (requiredSignature[i] == true && newEntitySignature[i] == false) {
          // p.second->mEntityIDs.erase(entityID);
          p.second->mEntityIDs.remove(entityID);
          break;
        }
      }

      if (i == requiredSignature.size()) {
        // p.second->mEntityIDs.insert(entityID);
        p.second->mEntityIDs.add(entityID, entityID);
      }
    }
  }

  template <typename EntityType> //
  void update(const EntityType &entity)
    requires CValidEntity<EntityType>
  {
    this->update(entity.getID());
  }

  void clear() noexcept { this->mSystems.clear(); }

  template <typename SystemType, typename... Args> //
  void execute(World &world, Args &&...args)
    requires CValidSystem<SystemType> &&
             CValidSystemFunction<decltype(SystemType::function)>
  {
    try {
      SystemType::function(
          world, this->mSystems.at(typenameStr<SystemType>())->mEntityIDs,
          std::forward<Args>(args)...);
    } catch (std::out_of_range &e) {
      throw std::runtime_error("SystemManager::execute: unregistered system");
    } catch (std::exception &e) {
      // Exception from SystemType::function (if any)
      throw e;
    }
  }

private:
  std::unordered_map<cString, std::unique_ptr<System>> mSystems = {};
};

} // namespace ecs

#endif // ECS_SOURCE_SYSTEM_HPP