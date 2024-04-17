#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Container/UDenseTypeTable.hpp"
#include "Entity/Entity.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig TConfig> //
class Group {
  using TArchetype = container::UDenseTypeTable<EntityID>;
  using TAdjList = // TODO: weak_ptr instead
      std::unordered_map<std::type_index, std::shared_ptr<TArchetype>>;

public:
  template <IsComponent... TComponents> //
  constexpr auto init() -> void {
    m_archetype.init<TComponents...>();
  }

public:
  template <IsComponent... TComponents> //
  constexpr auto addEntity(const EntityID &entityID, TComponents... components)
      -> void {
    m_archetype.add<TComponents...>(entityID, std::move(components)...);
  }

  //   template <IsComponent... TComponents> //
  //   constexpr auto yield(const EntityID &entityID) ->
  //   std::tuple<TComponents...> {
  //     std::tuple<TComponents...> components =
  //         std::make_tuple(m_archetype.at<TComponents>(entityID)...);
  //   }

  //   template <IsComponent TComponent> //
  //   constexpr auto moveEntityForward(const EntityID &entityID)
  //       -> std::vector<std::any> {
  //     std::tuple<TComponents...> components =
  //         std::make_tuple(m_archetype.at<TComponents>(entityID)...);
  //   }

  //   template <IsComponent TComponent> //
  //   constexpr auto moveEntityBackward(const EntityID &entityID)
  //       -> std::vector<std::any> {
  //     std::tuple<TComponents...> components =
  //         std::make_tuple(m_archetype.at<TComponents>(entityID)...);
  //   }

private:
  TArchetype m_archetype;
  TAdjList m_forward;
  TAdjList m_backward;
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP