#ifndef USHI_INCLUDE_USHI_IMPL_WORLD_HPP
#define USHI_INCLUDE_USHI_IMPL_WORLD_HPP

#include "Ushi/Core/Core.hpp"

#include "Ushi/Impl/Config/Config.hpp"

#include <atomic>

namespace ushi {

/**
 * @brief
 *
 * @tparam T_Config
 */
template <IsValidConfig T_Config = DefaultConfig> //
class World final {
public:
  static auto instance() -> std::shared_ptr<World>;

  constexpr World(const World &) noexcept = delete;
  constexpr World &operator=(const World &) noexcept = delete;

private:
  // static std::atomic<World *> worldInstance = nullptr;

private:
  constexpr World() noexcept = default;

private:
};

// template <IsValidConfig T_Config>
// std::atomic<World<T_Config> *> worldInstance = nullptr;

template <IsValidConfig T_Config>
auto World<T_Config>::instance() -> std::shared_ptr<World> {
  static std::atomic<std::shared_ptr<World>> worldInstance = nullptr;

  if (worldInstance.load() == nullptr) {
    // auto pWorld = new World{};
    worldInstance.store(std::shared_ptr<World>(new World{}));
    // worldInstance.load() = World{};
  }
  return worldInstance.load();
}

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_WORLD_HPP
