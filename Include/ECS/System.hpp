#ifndef ECS_INCLUDE_ECS_SYSTEM_HPP
#define ECS_INCLUDE_ECS_SYSTEM_HPP

#include "Base.hpp"
#include "Entity.hpp"

namespace ecs {

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

class SystemManager {
public:
  SystemManager() = default;
  ~SystemManager() = default;
  SystemManager(const SystemManager &) = delete;
  SystemManager(SystemManager &&) noexcept = default;
  SystemManager &operator=(const SystemManager &) = delete;
  SystemManager &operator=(SystemManager &&) noexcept = default;

  u64 getNSystems() const noexcept { return this->mSystems.size(); }

  template <typename SystemType> //
  bool exists() const noexcept
    requires CValidSystem<SystemType>
  {
    return this->mSystems.contains(typenameStr<SystemType>());
  }

  bool exists(cString typenameStr) const noexcept {
    return this->mSystems.contains(std::move(typenameStr));
  }

  /**
   * @brief System must be static, that is, the qualified condition does not
   * change. The second registeration of a system type will be ignore.
   * @throws std::bad_alloc from either std::unique_ptr or std::unordered_map
   */
  template <typename SystemType> //
  void reg(DynamicBitset qualifications = {}) noexcept(false)
    requires CValidSystem<SystemType>
  {
    auto typeStr = typenameStr<SystemType>();

    if (this->exists(typeStr)) {
      return; // already registered
    }

    System *pSystemBase = new SystemType();
    pSystemBase->mQualifications = std::move(qualifications);
    this->mSystems[typenameStr<SystemType>()] =
        std::unique_ptr<System>(std::move(pSystemBase));
  }

  /**
   * @throws std::out_of_range from std::unordered_map
   */
  template <typename SystemType> //
  std::unique_ptr<SystemType> get() const noexcept(false)
    requires CValidSystem<SystemType>
  {
    return std::static_pointer_cast<SystemType>(
        this->mSystems.at(typenameStr<SystemType>()));
  }

  /**
   * @throws std::out_of_range from std::unordered_map
   */
  template <typename SystemType> //
  const DynamicBitset &getQualifications() const noexcept(false)
    requires CValidSystem<SystemType>
  {
    return this->get<SystemType>()->mQualifications;
  }

  /**
   * @throws std::out_of_range from std::unordered_map
   */
  template <typename SystemType> //
  const DynamicBitset &setQualifications(DynamicBitset qualifications) const
      noexcept(false)
    requires CValidSystem<SystemType>
  {
    return this->get<SystemType>()->mQualifications = std::move(qualifications);
  }

  void remove(u64 entityID) noexcept {
    for (auto &p : this->mSystems) {
      p.second->mEntityIDs.remove(entityID);
    }
  }

  template <typename EntityType> //
  void remove(const EntityType &entity) noexcept
    requires CValidEntity<EntityType>
  {
    this->remove(entity.getID());
  }

  /**
   * @throws std::bad_alloc from either container::DynamicBitset or
   * container::SparseVector.
   */
  void update(u64 entityID,
              const DynamicBitset &newEntitySignature) noexcept(false) {
    for (auto &p : this->mSystems) {
      const DynamicBitset &requiredSignature = p.second->mQualifications;

      (requiredSignature & newEntitySignature) == requiredSignature
          ? p.second->mEntityIDs.add(entityID, entityID)
          : p.second->mEntityIDs.remove(entityID);
    }
  }

  /**
   * @throws std::bad_alloc from either container::DynamicBitset or
   * container::SparseVector.
   */
  template <typename EntityType> //
  void update(const EntityType &entity) noexcept(false)
    requires CValidEntity<EntityType>
  {
    this->update(entity.getID());
  }

  void clear() noexcept { this->mSystems.clear(); }

  /**
   * @throws std::out_of_range from std::unordered_map and any exceptions thrown
   * by user's implementation of System::function.
   */
  template <typename SystemType, typename... Args> //
  void execute(World &world, Args &&...args) noexcept(false)
    requires CValidSystem<SystemType> &&
             CValidSystemFunction<decltype(SystemType::function)>
  {
    SystemType::function(
        world, this->mSystems.at(typenameStr<SystemType>())->mEntityIDs,
        std::forward<Args>(args)...);
  }

private:
  std::unordered_map<cString, std::unique_ptr<System>> mSystems = {};
};

} // namespace ecs

#endif // ECS_INCLUDE_ECS_SYSTEM_HPP