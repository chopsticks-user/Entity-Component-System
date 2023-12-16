#ifndef ECS_SYSTEM_HPP
#define ECS_SYSTEM_HPP

#include "Base.hpp"
#include "Entity.hpp"

namespace ecs {

class World;

class System {
public:
  friend class SystemManager;

  System() = default;
  virtual ~System() = default;
  //   System(const System &) = delete;
  //   System(System &&) = default;
  //   System &operator=(const System &) = delete;
  //   System &operator=(System &&) = default;

  void function(...) const {
    throw std::runtime_error("System::function: null functionality system");
  }

  SparseVector<u64> entityIDs = {};

private:
  DynamicBitset mQualifications = {};
};

template <typename SystemType>
concept CValidSystem = std::copy_constructible<SystemType> &&
                       std::derived_from<SystemType, System> &&
                       !std::is_same_v<SystemType, System>;

class SystemManager final {
public:
  SystemManager() = default;
  ~SystemManager() = default;
  SystemManager(const SystemManager &) = delete;
  SystemManager(SystemManager &&) = default;
  SystemManager &operator=(const SystemManager &) = delete;
  SystemManager &operator=(SystemManager &&) = default;

  u64 getNSystems() const noexcept { return this->mSystems.size(); }

  template <typename SystemType> //
  void reg(DynamicBitset qualifications = {})
    requires CValidSystem<SystemType>
  {
    auto pSystem = std::make_shared<SystemType>();
    pSystem->mQualifications = std::move(qualifications);
    this->mSystems[typenameStr<SystemType>()] = pSystem;
  }

  // template <typename SystemType> //
  // void dereg()
  //   requires CValidSystem<SystemType>
  // {
  //   this->mSystems.erase(typenameStr<SystemType>());
  // }

  template <typename SystemType> //
  std::shared_ptr<SystemType> get() const
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

  // template <typename SystemType, typename EntityType> //
  // void add(const EntityType &entity)
  //   requires CValidSystem<SystemType> && CValidEntity<EntityType>
  // {
  //   const DynamicBitset &signature = entity.getSignature();
  //   if (!std::equal(signature.begin(), signature.end(),
  //                   this->getQualifications<SystemType>().begin())) {
  //     throw std::runtime_error("SystemManaged::add: unqualified entity");
  //   }
  // }

  template <typename SystemType> //
  void remove(u64 entityID)
    requires CValidSystem<SystemType>
  {
    this->get<SystemType>()->mQualifiedEntitiesIDs.remove(entityID);
  }

  void remove(u64 entityID) {
    for (auto &p : this->mSystems) {
      p.second->entityIDs.remove(entityID);
    }
  }

  template <typename SystemType, typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidSystem<SystemType> && CValidEntity<EntityType>
  {
    this->remove<SystemType>(entity.getID());
  }

  template <typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidEntity<EntityType>
  {
    this->remove(entity.getID());
  }

  void clear() { this->mSystems.clear(); }

private:
  std::unordered_map<cString, std::shared_ptr<System>> mSystems = {};
};

} // namespace ecs

#endif // ECS_SYSTEM_HPP