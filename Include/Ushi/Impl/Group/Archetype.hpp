#ifndef USHI_INCLUDE_USHI_IMPL_ARCHETYPE_HPP
#define USHI_INCLUDE_USHI_IMPL_ARCHETYPE_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Container/TypeErasedVector.hpp"
#include "Entity/Entity.hpp"

#include <span>

namespace ushi {
namespace internal {
namespace impl {

template <IsConfig TConfig> //
class Archetype {
  using TArhetypeInfo = std::vector<std::pair<ComponentRecordID, u64>>;

public:
  constexpr Archetype() = default;

  constexpr Archetype(const TArhetypeInfo &archetypeInfo) {
    for (auto [componentID, componentSize] : archetypeInfo) {
      m_componentTable.try_emplace(componentID,
                                   container::TypeErasedVector{componentSize});
    }
  }

public:
  constexpr auto size() const noexcept -> u64 { return m_entityIDs.size(); }

  constexpr auto level() const noexcept -> u64 {
    return m_componentTable.size();
  }

  constexpr auto addEntity(const ComponentRecord<TConfig> &componentRecord,
                           const EntityID &entityID,
                           IsComponent auto... components) -> void {
    if (!m_entityIDs.insert(entityID).second) {
      throw std::runtime_error(
          "Archetype: !m_entityIDs.insert(entityID).second");
    }

    (m_componentTable
         .at(componentRecord.template getIndex<decltype(components)>())
         .add(std::move(components)),
     ...);
  }

  //   constexpr auto receive() -> void {}

private:
  std::unordered_set<EntityID> m_entityIDs = {};
  std::unordered_map<ComponentRecordID, container::TypeErasedVector>
      m_componentTable = {};
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_ARCHETYPE_HPP