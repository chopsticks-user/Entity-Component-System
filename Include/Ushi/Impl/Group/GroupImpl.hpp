#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Container/TypeErasedVector.hpp"
#include "Entity/Entity.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig TConfig> //
class Group {
  using TCompatibleEntityIDs = std::vector<EntityID>;
  using TArchetype =
      std::unordered_map<ComponentRecordID, container::TypeErasedVector>;
  using TArchetypeInfo = std::vector<std::pair<ComponentRecordID, u64>>;

public:
  constexpr Group() noexcept = default;

  explicit constexpr Group(const TArchetypeInfo &archetypeInfo)
      : m_entityIDs{}, m_archetype{} {
    for (const auto &[componentID, elementSize] : archetypeInfo) {
      m_archetype.try_emplace(componentID,
                              container::TypeErasedVector{elementSize});
    }
  }

  explicit constexpr Group(const TCompatibleEntityIDs &entityIDs,
                           const TArchetype &archetype)
      : m_entityIDs{entityIDs}, m_archetype{archetype} {}

public:
  constexpr auto level() const noexcept -> u64 { return m_archetype.size(); }

  constexpr auto archetypeInfo() const noexcept -> TArchetypeInfo {
    TArchetypeInfo info(m_archetype.size());
    for (const auto &[componentID, componentVector] : m_archetype) {
      info.push_back(
          std::make_pair(componentID, componentVector.elementSize()));
    }
    return info;
  }

  template <IsComponent... TComponents> //
  constexpr auto addEntity(const EntityID &entityID) -> void {
    m_entityIDs.push_back(entityID);
  }

private:
private:
  TCompatibleEntityIDs m_entityIDs{};
  TArchetype m_archetype = {};
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP