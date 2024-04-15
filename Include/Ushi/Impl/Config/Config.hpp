#ifndef USHI_INCLUDE_USHI_IMPL_CONFIG_HPP
#define USHI_INCLUDE_USHI_IMPL_CONFIG_HPP

#include "Core/Core.hpp"

#include "Entity/EntityID.hpp"

namespace ushi {
namespace internal {
namespace impl {

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
  { core::FirstTemplateArg<T_Signature>::value } -> std::convertible_to<u64>;
  s1 & s2;
  s1 | s2;
  s1 ^ s2;
  s1[core::FirstTemplateArg<T_Signature>::value - 1];
} && core::FirstTemplateArg<T_Signature>::value > 0;

template <typename T_Config>
concept IsConfig = requires {
  requires IsSignature<typename T_Config::SignatureType>;
  requires IsEIDGenerator<typename T_Config::EIDGeneratorType>;
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_CONFIG_HPP