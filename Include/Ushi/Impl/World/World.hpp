#ifndef USHI_INCLUDE_USHI_IMPL_WORLD_HPP
#define USHI_INCLUDE_USHI_IMPL_WORLD_HPP

#include "Core/Core.hpp"

#include "Config/Config.hpp"

namespace ushi {

template <IsConfig T_Config = DefaultConfig> //
class World final {
  using T_EntityManager = EntityManager<T_Config>;
  using T_ComponentTable = ComponentTable<T_Config>;
  using T_SystemManager = SystemManager<T_Config>;

public:
  static auto instance() -> std::shared_ptr<World>;

private:
  constexpr World() = default;

private:
  T_EntityManager m_entityManager = {};
  T_ComponentTable m_componentTable = {};
  T_SystemManager m_systemManager = {};
};

template <IsConfig T_Config>
auto World<T_Config>::instance() -> std::shared_ptr<World> {
  static std::atomic<std::shared_ptr<World>> worldInstance = nullptr;

  if (worldInstance.load() == nullptr) {
    worldInstance.store(std::shared_ptr<World>(new World{}));
  }
  return worldInstance.load();
}

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_WORLD_HPP
