#ifndef TORA_INCLUDE_TORA_CORE_UTILS_HPP
#define TORA_INCLUDE_TORA_CORE_UTILS_HPP

#include "Forward.hpp"

#include <tuple>
#include <typeinfo>

namespace tora {

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

template <typename FuncType, typename Arg1Type, typename Arg2Type,
          typename... Args> //
struct First2ArgTypes<FuncType(Arg1Type, Arg2Type, Args...)> {
  using type1 = Arg1Type;
  using type2 = Arg2Type;
};

#define TORA_SIMPLE_ENTITY_CLASS(EntityTypename)                               \
  class EntityTypename : public tora::Entity {                                 \
    using Entity::Entity;                                                      \
  }

} // namespace tora

#endif // TORA_INCLUDE_TORA_CORE_UTILS_HPP