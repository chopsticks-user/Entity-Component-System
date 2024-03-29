#ifndef USHI_INCLUDE_USHI_IMPL_COMPONENT_RECORD_HPP
#define USHI_INCLUDE_USHI_IMPL_COMPONENT_RECORD_HPP

#include "Ushi/Core/Core.hpp"

#include "Ushi/Container/UnorderedDenseMap.hpp"
#include "Ushi/Impl/Config/Config.hpp"

#include <typeindex>
#include <typeinfo>

namespace ushi {

template <IsConfig T_Config> //
class ComponentRecord final {
public:
  // static constexpr u64 maxComponents =
  //     FirstTemplateArg<typename T_Config::DefaultConfig>::value;
  static constexpr u64 maxComponents = 64;

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

  /**
   * @brief
   *
   * @tparam T_Component
   */
  template <IsComponent T_Component> //
  constexpr auto record() -> void {
    auto tpidx = m_getkey<T_Component>();
    // TODO: UnorderedDenseMap.try_emplace
    if (!m_record.contains(tpidx)) {
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
    auto tpidx = m_getkey<T_Component>();
    if (!m_record.contains(tpidx)) {
      return maxComponents;
    }
    return m_record[tpidx];
  }

private:
  template <IsComponent T_Component> //
  constexpr auto m_getkey() const noexcept -> std::type_index {
    return std::type_index{typeid(T_Component)};
  }

private:
  UnorderedDenseMap<std::type_index, u64> m_record = {};
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_COMPONENT_RECORD_HPP