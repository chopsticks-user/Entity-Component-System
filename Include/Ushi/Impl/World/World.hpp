#ifndef USHI_INCLUDE_USHI_IMPL_WORLD_HPP
#define USHI_INCLUDE_USHI_IMPL_WORLD_HPP

#include "Ushi/Core/Core.hpp"

#include "Ushi/Impl/Config/Config.hpp"

namespace ushi {

// TODO: should make this class singleton
template <typename T_Config = DefaultConfig>
  requires IsValidConfig<T_Config>
class World {
public:
  using Config = T_Config;

private:
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_WORLD_HPP
