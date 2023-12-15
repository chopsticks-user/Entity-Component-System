#ifndef ECS_SYSTEM_HPP
#define ECS_SYSTEM_HPP

#include "Base.hpp"

namespace ecs {

class World;

class System {
  friend class SystemManager;

public:
  System() = default;
  virtual ~System() = default;
  //   System(const System &) = delete;
  //   System(System &&) = default;
  //   System &operator=(const System &) = delete;
  //   System &operator=(System &&) = default;

  void function() {}

private:
  std::vector<u64> qualifiedEntitiesIDs = {};
};

template <typename SystemType>
concept CValidSystem = std::copy_constructible<SystemType> &&
                       std::derived_from<SystemType, System> &&
                       !std::is_same_v<SystemType, System>;

class SystemManager final {};

} // namespace ecs

#endif // ECS_SYSTEM_HPP