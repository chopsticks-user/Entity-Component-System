#ifndef TORA_INCLUDE_TORA_CORE_TEMPLATE_MAP_HPP
#define TORA_INCLUDE_TORA_CORE_TEMPLATE_MAP_HPP

#include "Forward.hpp"
#include "Utils.hpp"

#include <functional>
#include <unordered_map>

namespace tora {

template <template <typename> class TStorage> //
class InstanceStorageHashTable {
public:
  InstanceStorageHashTable() noexcept = delete;
  InstanceStorageHashTable(
      std::function<void(TStorage &)> addPolicy,
      std::function<void(TStorage &)> lookupPolicy) noexcept
      : m_addPolicy{std::move(addPolicy)},
        m_lookupPolicy{std::move(lookupPolicy)} {}

  template <typename T> //
  auto add(T v) noexcept -> void {
    auto it = m_table.find(typeid(T).name());
    if (it == m_table.end()) {
      auto &ref = m_table[typeid(T).name()];
      ref = std::make_any<TStorage<T>>();
    }
    m_addPolicy(std::any_cast<TStorage<T> &>(m_table[typeid(T).name()]));
  }

  template <typename T> //
  auto get() noexcept -> std::optional<T> {
    auto it_lookup = m_table.find(typeid(T).name());
    if (it_lookup == m_table.end()) {
      return {};
    }
    return {std::any_cast<TStorage<T> &>(it_lookup->second)};
  }

private:
  template <typename T> //
  auto lookup() -> std::optional<TStorage<T> &> {
    auto it_lookup = m_table.find(typeid(T).name());
    if (it_lookup == m_table.end()) {
      return {};
    }
    return {std::any_cast<TStorage<T> &>(it_lookup->second)};
  }

private:
  std::unordered_map<const char *, std::any> m_table;
  std::function<void(TStorage &)> m_addPolicy;
  std::function<void(TStorage &)> m_lookupPolicy;
};

} // namespace tora

#endif // TORA_INCLUDE_TORA_CORE_TEMPLATE_MAP_HPP