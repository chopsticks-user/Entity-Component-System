#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Container/PolyTypeTable.hpp"
#include "Entity/Entity.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig TConfig> //
class Group {
  using TArchetype = container::PolyTypeTable<EntityID, Component>;
  using TAdjList = // TODO: weak_ptr instead
      std::unordered_map<std::type_index, std::shared_ptr<TArchetype>>;

  using TComponentPackage =
      std::unordered_map<std::type_index, std::unique_ptr<Component>>;

public:
  // TODO: number of components < maxComponents
  // template <IsComponent... TComponents> //
  // constexpr auto init() -> void {
  //   m_archetype.init<TComponents...>();
  // }

public:
  [[nodiscard]] constexpr auto level() const noexcept -> u64 {
    return m_archetype.nTypes();
  }

  [[nodiscard]] constexpr auto size() const noexcept -> u64 {
    return m_archetype.size();
  }

  template <IsComponent... TComponents> //
  constexpr auto addEntityWith(const EntityID &entityID,
                               TComponents... components) -> void {
    m_archetype.addWithTypes<TComponents...>(entityID,
                                             std::move(components)...);
  }

  constexpr auto receive(const EntityID &entityID,
                         TComponentPackage &components) -> void {
    m_archetype.addWithPackage(entityID, components);
  }

  [[nodiscard]] constexpr auto transfer(const EntityID &entityID)
      -> TComponentPackage {
    return m_archetype.popAt(entityID);
  }

private:
  TArchetype m_archetype;
  TAdjList m_forward;
  TAdjList m_backward;
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP