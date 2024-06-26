#ifndef USHI_INCLUDE_USHI_CORE_CONCEPTS_HPP
#define USHI_INCLUDE_USHI_CORE_CONCEPTS_HPP

#include "Typenames.hpp"
#include "Utils.hpp"

#include <concepts>
#include <type_traits>
#include <typeindex>

namespace ushi {
namespace internal {
namespace core {

// https://en.cppreference.com/w/cpp/language/constraints
template <typename T>
concept IsHashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template <typename T_Derived, typename T_Base>
concept IsChildOf =
    !std::same_as<T_Derived, T_Base> && std::derived_from<T_Derived, T_Base>;

} // namespace core
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_CORE_CONCEPTS_HPP
