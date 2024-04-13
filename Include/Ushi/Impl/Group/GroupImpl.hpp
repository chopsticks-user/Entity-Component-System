#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Container/UnorderedDenseMap.hpp"
#include "Container/VectorWrapper.hpp"
#include "Entity/Entity.hpp"

namespace ushi {

template <IsConfig T_Config> //
class GroupFactory;

template <IsConfig T_Config> //
class Group final {
  using T_Table = UnorderedDenseMap<u64, std::shared_ptr<VectorWrapperBase>>;

  friend class GroupFactory<T_Config>;

public:
  constexpr Group(Group &&) noexcept = default;
  Group(const Group &) = delete;
  constexpr auto operator=(Group &&) noexcept -> Group & = default;
  auto operator=(const Group &) -> Group & = delete;

public:
  [[nodiscard]] constexpr auto level() const noexcept -> u64 {
    return m_table.size();
  }

  [[nodiscard]] constexpr auto signature() const noexcept
      -> T_Config::SignatureType & {
    return m_signature;
  }

  // constexpr auto add(IsEntity entity) -> void {}

private:
  Group() = default;

private:
  T_Table m_table;
  T_Config::SignatureType m_signature;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
