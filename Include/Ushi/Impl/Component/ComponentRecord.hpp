#ifndef USHI_INCLUDE_USHI_IMPL_COMPONENT_RECORD_HPP
#define USHI_INCLUDE_USHI_IMPL_COMPONENT_RECORD_HPP

#include "Ushi/Core/Core.hpp"

#include "Ushi/Container/UnorderedDenseMap.hpp"
#include "Ushi/Impl/Config/Config.hpp"

#include <typeindex>

namespace ushi {

template <IsValidConfig T_Config> //
class ComponentRecord final {
public:
  static constexpr u64 maxComponents =
      FirstTemplateArg<typename T_Config::DefaultConfig>::value;

public:
  constexpr ComponentRecord() noexcept = default;

  constexpr ~ComponentRecord() noexcept = default;

  constexpr ComponentRecord(ComponentRecord &&) noexcept = default;

  constexpr ComponentRecord(const ComponentRecord &) noexcept = delete;

  constexpr auto operator=(ComponentRecord &&) noexcept
      -> ComponentRecord & = default;

  constexpr auto operator=(const ComponentRecord &) noexcept
      -> ComponentRecord & = delete;

  constexpr auto size() const noexcept -> u64 { return m_record.size(); }

  constexpr auto full() const noexcept -> bool {
    return m_record.size() >= maxComponents;
  }

  template <IsComponent T_Component> //
  constexpr auto record() -> void {
    auto tpidx = m_typeindex<T_Component>();
    // TODO: UnorderedDenseMap.try_emplace
    if (!m_record.exists(tpidx)) {
      m_record.add(tpidx, m_record.size());
    }
  }

  /**
   *
   * @tparam T_Component
   * @return u64
   */
  template <IsComponent T_Component> //
  constexpr auto getIndex() const -> u64 {
    auto tpidx = m_typeindex<T_Component>();
    if (!m_record.exists(tpidx)) {
      return maxComponents;
    }
    return m_record[tpidx];
  }

private:
  template <IsComponent T_Component> //
  constexpr auto m_typeindex() const noexcept -> std::type_index {
    return std::type_index{typeid(T_Component)};
  }

private:
  UnorderedDenseMap<std::type_index, u64> m_record = {};
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_COMPONENT_RECORD_HPP