#ifndef USHI_INCLUDE_USHI_IMPL_CONFIG_HPP
#define USHI_INCLUDE_USHI_IMPL_CONFIG_HPP

#include "Ushi/Core/Core.hpp"

#include <bitset>

namespace ushi {

/**
 * @brief
 *
 */
struct DefaultConfig {
  using SignatureType = std::bitset<64>;
};

// template <typename T_Config>
// concept IsValidConfig =
//     std::derived_from<T_Config, Config> && !std::same_as<T_Config, Config>;

// TODO:
// template <typename T_Signature>
// concept IsValidSignature = ;

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_CONFIG_HPP