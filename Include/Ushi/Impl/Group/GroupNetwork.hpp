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
  using T_GroupFactory = GroupFactory<T_Config>;
  using T_BaseGroupContainer =
      std::unordered_map<ComponentRecordID, std::shared_ptr<Group<T_Config>>>;

public:
private:
  T_GroupFactory m_groupFactory;
  T_BaseGroupContainer m_baseGroups;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP
