#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "GroupFactory.hpp"
#include "GroupImpl.hpp"

namespace ushi {

template <IsConfig T_Config> //
class GroupNetwork final {
  using T_Group = Group<T_Config>;
  using T_GroupFactory = GroupFactory<T_Config>;
  using T_BaseGroupContainer =
      std::unordered_map<ComponentRecordID, std::shared_ptr<T_Group>>;
  using T_Signature = T_Config::SignatureType;

  using T_ComponentRecord = ComponentRecord<T_Config>;

public:
  template <IsComponent... T_Components> //
  constexpr auto addGroup() -> void {}

  template <IsComponent... T_Components> //
  constexpr auto addEntity() -> void {}

  template <IsComponent T_Components> //
  constexpr auto addComponentToEntity() -> void {}

  template <IsComponent T_Component> //
  constexpr auto addBaseGroup(const T_ComponentRecord &componentRecord)
      -> void {
    if (!componentRecord.template contains<T_Component>()) {
      throw std::runtime_error("Component type has not been registered.");
    }

    m_baseGroups[componentRecord.template getIndex<T_Component>()] =
        std::make_shared<T_Group>(
            m_groupFactory.template create<T_Component>(componentRecord));
  }

private:
  template <IsComponent... T_Components> //
  constexpr auto m_findMostSuitableGroup() {}

private:
  T_GroupFactory m_groupFactory;
  T_BaseGroupContainer m_baseGroups;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP
