#ifndef USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
#define USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Container/UnorderedDenseMap.hpp"

namespace ushi {

class VectorWrapperBase {
public:
  virtual ~VectorWrapperBase() noexcept = default;
};

template <typename T> //
class VectorWrapper final : public VectorWrapperBase {
public:
  constexpr auto operator()() noexcept -> std::vector<T> & { return m_vector; }

private:
  std::vector<T> m_vector;
};

template <typename T> //
using Vector = VectorWrapper<T>;

template <IsConfig T_Config> //
class GroupFactory;

template <IsConfig T_Config> //
class Group {
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

private:
  Group() = default;

private:
  T_Table m_table;
  T_Config::SignatureType m_signature;
};

template <IsConfig T_Config> //
class GroupFactory {
public:
  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto create(const ComponentRecord<T_Config> &record)
      -> Group<T_Config> {
    Group<T_Config> group;
    (group.m_table.add(record.template getIndex<T_Components>(), {}), ...);
    group.m_signature = record.template signature<T_Components...>();
    return group;
  }
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_GROUP_IMPL_HPP
