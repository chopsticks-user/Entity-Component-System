#ifndef TORA_INCLUDE_TORA_BASE_HPP
#define TORA_INCLUDE_TORA_BASE_HPP

#if __cplusplus >= 202002L
#include <Nezumi/DynamicBitset.hpp>
#include <Nezumi/SparseVector.hpp>

#include <algorithm>
#include <concepts>
#include <memory>
#include <numeric>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tora {

constexpr bool cppAtLeast14 = __cplusplus >= 201402L;
constexpr bool cppAtLeast17 = __cplusplus >= 201703L;
constexpr bool cppAtLeast20 = __cplusplus >= 202002L;
constexpr bool cpp14 = cppAtLeast14 && !cppAtLeast17;
constexpr bool cpp17 = cppAtLeast17 && !cppAtLeast20;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef bool b8;
typedef uint32_t b32;

typedef const char *cString;

using nezumi::DynamicBitset;
using nezumi::ISparseVector;
using nezumi::SparseVector;
typedef SparseVector<u64> UniqueIDContainer;

template <typename Type> //
cString typenameStr() noexcept {
  return typeid(Type).name();
}

template <u64 Index, typename... Args> //
void iterateTuple(std::tuple<Args...> &tp, auto func) {
  func(std::get<Index>(tp));
  if constexpr (Index + 1 != sizeof...(Args)) {
    iterateTuple<Index + 1>(tp, func);
  }
}

template <typename> //
struct First2ArgTypes;

template <typename FuncType, typename Arg1Type, typename Arg2Type,
          typename... Args> //
struct First2ArgTypes<FuncType(Arg1Type, Arg2Type, Args...)> {
  using type1 = Arg1Type;
  using type2 = Arg2Type;
};

class Entity;
class EntityManager;

class Component;
class ComponentTable;

class System;
class SystemManager;

class World;

#define TORA_SIMPLE_ENTITY_CLASS(EntityTypename)                               \
  class EntityTypename : public tora::Entity {                                 \
    using Entity::Entity;                                                      \
  }

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
#else  // C++17 or older
static_assert(__cplusplus >= 202002L, "Nezumi library requires C++20 or newer");
#endif // C++20 or newer

#endif // TORA_INCLUDE_TORA_BASE_HPP