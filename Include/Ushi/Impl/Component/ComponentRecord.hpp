#ifndef USHI_INCLUDE_USHI_IMPL_COMPONENT_RECORD_HPP
#define USHI_INCLUDE_USHI_IMPL_COMPONENT_RECORD_HPP

#include "Core/Core.hpp"

#include "Config/Config.hpp"
#include "Container/UnorderedDenseMap.hpp"

namespace ushi {
namespace internal {
namespace impl {

using ComponentRecordID = u64;

template <IsConfig T_Config> //
class ComponentRecord final {
public:
  static constexpr u64 maxComponents =
      core::FirstTemplateArg<typename T_Config::SignatureType>::value;

public:
  constexpr ComponentRecord() noexcept = default;

  constexpr ~ComponentRecord() noexcept = default;

  constexpr ComponentRecord(ComponentRecord &&) noexcept = default;

  constexpr ComponentRecord(const ComponentRecord &) noexcept = delete;

  constexpr auto operator=(ComponentRecord &&) noexcept
      -> ComponentRecord & = default;

  constexpr auto operator=(const ComponentRecord &) noexcept
      -> ComponentRecord & = delete;

  constexpr auto size() const noexcept -> ComponentRecordID {
    return m_record.size();
  }

  constexpr auto full() const noexcept -> bool {
    return m_record.size() >= maxComponents;
  }

  template <IsComponent T_Component> //
  constexpr auto regster() -> void {
    if (!full()) {
      m_record.try_emplace(typeid(T_Component), m_record.size());
    }
  }

  template <IsComponent T_Component> //
  constexpr auto getIndex() const noexcept -> ComponentRecordID {
    auto it = m_record.find(typeid(T_Component));
    if (it == m_record.end()) {
      return maxComponents;
    }
    return it->second;
  }

  template <IsComponent T_Component> //
  constexpr auto contains() const noexcept -> bool {
    return getIndex<T_Component>() < maxComponents;
  }

  template <IsComponent... T_Components> //
  [[nodiscard]] constexpr auto signature() const noexcept
      -> T_Config::SignatureType {
    typename T_Config::SignatureType signature;
    (m_setSignatureBit<T_Components>(signature, getIndex<T_Components>()), ...);
    return signature;
  }

private:
  template <IsComponent T_Components> //
  constexpr auto m_setSignatureBit(T_Config::SignatureType &signature,
                                   u64 index) const noexcept -> void {
    signature[index] = true;
  }

  template <IsComponent T_Component> //
  constexpr auto m_getkey() const noexcept -> std::type_index {
    return std::type_index{typeid(T_Component)};
  }

private:
  std::unordered_map<std::type_index, ComponentRecordID> m_record = {};
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_COMPONENT_RECORD_HPP
