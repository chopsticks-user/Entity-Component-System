#ifndef TORA_INCLUDE_TORA_CORE_CONCEPTS_HPP
#define TORA_INCLUDE_TORA_CORE_CONCEPTS_HPP

#include "Forward.hpp"
#include "Utils.hpp"

#include <concepts>

namespace tora {

template <typename EntityType>
concept CValidEntity = std::move_constructible<EntityType> &&
                       std::derived_from<EntityType, Entity> &&
                       !std::is_same_v<EntityType, Entity>;

template <typename ComponentType>
concept CValidComponent = std::copy_constructible<ComponentType> &&
                          std::derived_from<ComponentType, Component> &&
                          !std::is_same_v<ComponentType, Component>;

template <typename SystemType>
concept CValidSystem = std::move_constructible<SystemType> &&
                       std::derived_from<SystemType, System> &&
                       !std::is_same_v<SystemType, System>;

template <typename FunctionType>
concept CValidSystemFunction =
    std::is_same_v<typename First2ArgTypes<FunctionType>::type1, World &> &&
    std::is_same_v<typename First2ArgTypes<FunctionType>::type2,
                   const UniqueIDContainer &>;

} // namespace tora

#endif // TORA_INCLUDE_TORA_CORE_CONCEPTS_HPP