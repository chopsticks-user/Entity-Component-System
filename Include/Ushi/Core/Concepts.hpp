#ifndef USHI_INCLUDE_USHI_CORE_CONCEPTS_HPP
#define USHI_INCLUDE_USHI_CORE_CONCEPTS_HPP

#include "Forward.hpp"
#include "Utils.hpp"

#include <concepts>

namespace ushi {

// https://en.cppreference.com/w/cpp/language/constraints
template <typename T>
concept Hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

// template <typename EntityType>
// concept CValidEntity = std::move_constructible<EntityType> &&
//                        std::derived_from<EntityType, Entity> &&
//                        !std::is_same_v<EntityType, Entity>;
//
// template <typename ComponentType>
// concept CValidComponent = std::copy_constructible<ComponentType> &&
//                           std::derived_from<ComponentType, Component> &&
//                           !std::is_same_v<ComponentType, Component>;
//
// template <typename SystemTye>
// concept CValidSystem = std::move_constructible<SystemType> &&
//                        std::derived_from<SystemType, System> &&
//                        !std::is_same_v<SystemType, System>;
//
// template <typename FunctionType>
// concept CValidSystemFunction =
//     std::is_same_v<typename First2ArgTypes<FunctionType>::type1, World &> &&
//     std::is_same_v<typename First2ArgTypes<FunctionType>::type2,
//                    const UniqueIDContainer &>;

} // namespace ushi

#endif // USHI_INCLUDE_USHI_CORE_CONCEPTS_HPP
