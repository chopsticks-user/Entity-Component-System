#ifndef TORA_INCLUDE_TORA_CORE_UTILS_HPP
#define TORA_INCLUDE_TORA_CORE_UTILS_HPP

#include "Forward.hpp"

#include <tuple>

namespace tora {

template <u64 Index, typename... Args> //
void iterateTuple(std::tuple<Args...> &tp, auto func) {
  func(std::get<Index>(tp));
  if constexpr (Index + 1 != sizeof...(Args)) {
    iterateTuple<Index + 1>(tp, func);
  }
}

struct TupleCatBase {};
// https://stackoverflow.com/questions/53394100/concatenating-tuples-as-types
template <class, class> struct TupleCat;
template <class... First, class... Second>
struct TupleCat<std::tuple<First...>, std::tuple<Second...>>
    : public TupleCatBase {
  using type = std::tuple<First..., Second...>;
};
//

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