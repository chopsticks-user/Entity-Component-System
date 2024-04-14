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
  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto
  create(const ComponentRecord<T_Config> &componentRecord) -> Group<T_Config> {
    Group<T_Config> group;
    (group.m_table.add(componentRecord.template getIndex<T_Components>(),
                       std::make_shared<VectorWrapper<T_Components>>()),
     ...);
    group.m_signature = componentRecord.template signature<T_Components...>();
    return group;
  }
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_FACTORY_HPP
