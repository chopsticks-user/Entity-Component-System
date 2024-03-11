#ifndef TORA_INCLUDE_TORA_CORE_TYPE_MAP_HPP
#define TORA_INCLUDE_TORA_CORE_TYPE_MAP_HPP

#include "Forward.hpp"
#include "SparseVector.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <any>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <ranges>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace tora {

template <template <typename> class TAllocatorClass = std::allocator> //
class TypeMap {
  // TODO: Raw pointers with custom allocators
  // TODO: Optional exceptions
  using TTableKey = std::type_index;

  using TTableAllocator =
      TAllocatorClass<std::pair<const TTableKey, SparseVectorBase *>>;

  //* Use raw pointers for direct access
  using TTable = typename std::unordered_map<
      TTableKey, SparseVectorBase *, std::hash<std::type_index>,
      std::equal_to<std::type_index>, TTableAllocator>;

  template <typename TData> //
  using TSparseVector = SparseVector<TData, TAllocatorClass>;

public:
  constexpr TypeMap() noexcept = default;
  TypeMap(const TypeMap &) = delete;
  TypeMap &operator=(const TypeMap &) = delete;
  ~TypeMap() noexcept { clear(); }

  constexpr auto nTypes() const noexcept -> u64 { return m_table.size(); }

  template <typename T> //
  constexpr auto nElements() const noexcept -> u64 {
    const TSparseVector<T> *ptr = getSparseVectorPtr<T>();
    return ptr == nullptr ? 0 : ptr->size();
  }

  constexpr auto nElements() const noexcept -> u64 {
    u64 count = 0;
    for (const SparseVectorBase *const ptr : m_table | std::views::values) {
      count += ptr->size();
    }
    return count;
  }

  constexpr auto empty() const noexcept -> bool { return nTypes() == 0; }

  template <typename T> //
  constexpr auto empty() const noexcept -> bool {
    const TSparseVector<T> *ptr = getSparseVectorPtr<T>();
    return ptr == nullptr ? true : ptr->size() == 0;
  }

  template <typename T> //
  constexpr auto exists() const noexcept -> bool {
    return getSparseVectorPtr<T>() != nullptr;
  }

  template <typename T> //
  constexpr auto exists(u64 id) const noexcept -> bool {
    const TSparseVector<T> *ptr = getSparseVectorPtr<T>();
    return ptr == nullptr ? false : ptr->exists(id);
  }

  // TODO: remove duplicated queries
  template <typename T> //
  constexpr auto add() noexcept -> void {
    tryEmplace<T>();
  }

  template <typename T> //
  constexpr auto add(u64 id, T v) noexcept -> void {
    //* This static down cast is safe
    TSparseVector<T> *ptr =
        static_cast<TSparseVector<T> *>(tryEmplace<T>()->second);
    ptr->add(id, v);
  }

  // template <typename T> //
  // constexpr auto get() -> const TSparseVector<T> & {
  //   return getSparseVectorPtr<T>();
  // }

  template <typename T> //
  constexpr auto get(u64 id) -> T & {
    TSparseVector<T> *ptr = getSparseVectorPtr<T>();
    if (ptr == nullptr) {
      throw std::runtime_error("TypeMap::get()");
    }
    return getSparseVectorPtr<T>()->operator[](id);
  }

  template <typename T> //
  constexpr auto remove() noexcept -> void {
    auto it = getEntry<T>();
    if (it == m_table.end()) {
      return;
    }
    delete it->second;
    m_table.erase(it);
  }

  template <typename T> //
  constexpr auto remove(u64 id) noexcept -> void {
    TSparseVector<T> *ptr = getSparseVectorPtr<T>();
    if (ptr == nullptr) {
      return;
    }
    ptr->remove(id);
  }

  constexpr auto clear() noexcept -> void {
    for (SparseVectorBase *const ptr : m_table | std::views::values) {
      delete ptr;
    }
    m_table.clear();
  }

  template <typename T> //
  constexpr auto clear() noexcept -> void {
    TSparseVector<T> *ptr = getSparseVectorPtr<T>();
    if (ptr == nullptr) {
      return;
    }
    ptr->clear();
  }

  constexpr auto reset() noexcept -> void {
    for (SparseVectorBase *const ptr : m_table | std::views::values) {
      ptr->clear();
    }
  }

private:
  template <typename T> //
  auto getSparseVectorPtr() noexcept -> TSparseVector<T> * {
    auto it = m_table.find(typeid(T));
    if (it == m_table.end()) {
      return nullptr;
    }
    //* This static down cast is safe
    return static_cast<TSparseVector<T> *>(it->second);
  }

  template <typename T> //
  auto getSparseVectorPtr() const noexcept -> const TSparseVector<T> * {
    auto it = m_table.find(typeid(T));
    if (it == m_table.end()) {
      return nullptr;
    }
    //* This static down cast is safe
    return static_cast<TSparseVector<T> *>(it->second);
  }

  template <typename T> //
  auto getEntry() noexcept -> TTable::iterator {
    return m_table.find(typeid(T));
  }

  template <typename T> //
  auto tryEmplace() -> TTable::iterator {
    return m_table.try_emplace(typeid(T), new TSparseVector<T>{}).first;
  }

private:
  TTable m_table = {};
};

} // namespace tora

#endif // TORA_INCLUDE_TORA_CORE_TYPE_MAP_HPP