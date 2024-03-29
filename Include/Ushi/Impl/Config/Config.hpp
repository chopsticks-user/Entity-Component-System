#ifndef USHI_INCLUDE_USHI_IMPL_CONFIG_HPP
#define USHI_INCLUDE_USHI_IMPL_CONFIG_HPP

#include "Ushi/Core/Core.hpp"

#include "Ushi/Impl/Entity/EntityID.hpp"

#include <bitset>

namespace ushi {

/**
 * @brief
 *
 */
struct DefaultConfig final {
  using SignatureType = std::bitset<64>;
  using EIDGeneratorType = EntityIDGenerator<u64>;
};

// TODO:
template <typename T_Signature>
concept IsSignature = requires(T_Signature s1, T_Signature s2) {
  s1 & s2;
  s1 | s2;
  s1 ^ s2;
};

template <typename T_Config>
concept IsConfig = requires {
  requires IsSignature<typename T_Config::SignatureType>;
  requires IsEIDGenerator<typename T_Config::EIDGeneratorType>;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_CONFIG_HPP