#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_FACTORY_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_FACTORY_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "GroupImpl.hpp"

namespace ushi {

template <IsConfig T_Config> //
class GroupFactory final {
public:
  constexpr GroupFactory(const ComponentRecord<T_Config> &record) noexcept
      : m_record{&record} {}

  constexpr GroupFactory(GroupFactory &&) noexcept = default;

  GroupFactory(const GroupFactory &) = delete;

  constexpr auto operator=(GroupFactory &&) noexcept
      -> GroupFactory & = default;

  constexpr auto operator=(const GroupFactory &) noexcept
      -> GroupFactory & = delete;

public:
  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto create() -> Group<T_Config> {
    Group<T_Config> group;
    (group.m_table.add(m_record->template getIndex<T_Components>(),
                       std::make_shared<VectorWrapper<T_Components>>()),
     ...);
    group.m_signature = m_record->template signature<T_Components...>();
    return group;
  }

private:
  const ComponentRecord<T_Config> *m_record;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_FACTORY_HPP
