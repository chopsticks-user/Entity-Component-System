#ifndef USHI_INCLUDE_USHI_IMPL_WORLD_HPP
#define USHI_INCLUDE_USHI_IMPL_WORLD_HPP

#include "Core/Core.hpp"

#include "Config/Config.hpp"

namespace ushi {

/**
 * @brief
 *
 * @tparam T_Config
 */
template <IsConfig T_Config = DefaultConfig> //
class World final {
public:
  static auto instance() -> std::shared_ptr<World>;

  constexpr World(const World &) noexcept = delete;
  constexpr World &operator=(const World &) noexcept = delete;

private:
  constexpr World() noexcept = default;

private:
};

/**
 * @brief
 *
 * @tparam T_Config
 * @return std::shared_ptr<World>
 */
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
