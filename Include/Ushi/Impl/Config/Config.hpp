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

// TODO:
template <typename T_Signature>
concept IsValidSignature = requires(T_Signature s1, T_Signature s2) {
  s1 & s2;
  s1 | s2;
  s1 ^ s2;
};

template <typename T_Config>
concept IsValidConfig =
    requires { requires IsValidSignature<typename T_Config::SignatureType>; };

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_CONFIG_HPP