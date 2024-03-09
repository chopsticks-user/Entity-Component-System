#ifndef TORA_INCLUDE_TORA_CORE_TYPE_MAP_HPP
#define TORA_INCLUDE_TORA_CORE_TYPE_MAP_HPP

#include "Forward.hpp"
#include "SparseVector.hpp"
#include "Utils.hpp"

#include <any>
#include <optional>
#include <unordered_map>

namespace tora {

template <template <typename> class TAllocatorClass = std::allocator> //
class TypeMap {
  typedef typename std::unordered_map<
      cString, std::any, std::hash<cString>, std::equal_to<cString>,
      TAllocatorClass<std::pair<const cString, std::any>>>
      TTable;

  template <typename TData> //
  using TSparseVector = SparseVector<TData, TAllocatorClass>;

public:
  constexpr auto nTypes() const noexcept -> u64 { return m_table.size(); }

  // constexpr auto nElements() const noexcept -> u64 {
  //   u64 result = 0;
  //   for (const auto &[type, container] : m_table) {
  //     result += std::any_cast<const TSparseVector<T> &>(container).size();
  //   }
  //   return result;
  // }

  template <typename T> //
  constexpr auto nElements() const -> u64 {
    return std::any_cast<const TSparseVector<T> &>(m_table.at(typeid(T).name()))
        .size();
  }

  constexpr auto empty() const noexcept -> bool { return nTypes() == 0; }

  template <typename T> //
  constexpr auto empty() const noexcept -> bool {
    return nElements<T>() == 0;
  }

  template <typename T> //
  constexpr auto exists(u64 id) const noexcept -> bool {
    auto it = m_table.find(typeid(T).name());
    if (it == m_table.end()) {
      return false;
    }
    return std::any_cast<const TSparseVector<T> &>(it->second).exists(id);
  }

  template <typename T> //
  constexpr auto add(u64 id, T v) noexcept -> void {
    auto it = m_table.find(typeid(T).name());
    if (it == m_table.end()) {
      auto &ref = m_table[typeid(T).name()];
      ref = std::make_any<TSparseVector<T>>();
    }
    std::any_cast<TSparseVector<T> &>(it->second).add(id, v);
  }

  template <typename T> //
  constexpr auto get() const -> const TSparseVector<T> & {
    return std::any_cast<const TSparseVector<T> &>(
        m_table.at(typeid(T).name()));
  }

  template <typename T> //
  constexpr auto get(u64 id) const -> const T & {
    return get<T>()[id];
  }

  template <typename T> //
  constexpr auto remove(u64 id) noexcept -> void {
    auto it = m_table.find(typeid(T).name());
    if (it == m_table.end()) {
      return;
    }
    std::any_cast<TSparseVector<T> &>(it->second).remove(id);
  }

  template <typename T> //
  constexpr auto clear() noexcept -> void {
    m_table.clear();
  }

private:
private:
  TTable m_table = {};
};

} // namespace tora

#endif // TORA_INCLUDE_TORA_CORE_TYPE_MAP_HPP