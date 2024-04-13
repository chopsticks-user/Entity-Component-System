#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP

#include "Core/Core.hpp"

#include "Config/Config.hpp"
#include "GroupFactory.hpp"
#include "GroupImpl.hpp"

namespace ushi {

template <IsConfig T_Config> //
class GroupNetwork final {
public:
private:
  GroupFactory<T_Config> m_groupFactory;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_NETWORK_HPP
