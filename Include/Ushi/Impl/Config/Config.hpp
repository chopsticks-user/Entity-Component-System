#ifndef USHI_INCLUDE_USHI_IMPL_CONFIG_HPP
#define USHI_INCLUDE_USHI_IMPL_CONFIG_HPP

#include "Ushi/Core/Core.hpp"

#include <bitset>

namespace ushi {

struct Config {
  static constexpr u64 maxComponents = 64;
  using SignatureType = std::bitset<maxComponents>;
};

template <typename T_Config>
concept IsValidConfig =
    std::derived_from<T_Config, Config> && !std::same_as<T_Config, Config>;

struct DefaultConfig : public Config {};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_CONFIG_HPP