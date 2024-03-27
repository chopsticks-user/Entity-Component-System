#ifndef TORA_INCLUDE_TORA_CORE_TYPE_MAP_HPP
#define TORA_INCLUDE_TORA_CORE_TYPE_MAP_HPP

#include "Forward.hpp"
#include "SparseVector.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <memory_resource>
#include <ranges>
#include <typeindex>
#include <unordered_map>

namespace tora {

class TypeMap {
  // TODO: Raw pointers with custom allocators
  // TODO: Optional exceptions
  using TableKeyType = std::type_index;
  using GeneralAllocatorType = std::pmr::polymorphic_allocator<std::byte>;

  //* Use raw pointers for direct access
  using TableType = std::pmr::unordered_map<TableKeyType, SparseVectorBase *>;

public:
  TypeMap(GeneralAllocatorType allocator = {})
      : m_table{}, m_allocator{std::move(allocator)} {}

  constexpr TypeMap(const TypeMap &) = delete;

  TypeMap(TypeMap &&other)
      : m_table{std::move(other.m_table)},
        m_allocator{std::move(other.m_allocator)} {}

  TypeMap(TypeMap &&other, GeneralAllocatorType allocator = {})
      : m_table{std::move(other.m_table)}, m_allocator{std::move(allocator)} {}

  constexpr TypeMap &operator=(const TypeMap &) = delete;

  ~TypeMap() noexcept { clear(); }

  auto nTypes() const noexcept -> u64 { return m_table.size(); }

  template <typename T> //
  constexpr auto nElements() const noexcept -> u64 {
    const SparseVector<T> *ptr = getSparseVectorPtr<T>();
    return ptr == nullptr ? 0 : ptr->size();
  }

  auto nElements() const noexcept -> u64 {
    u64 count = 0;
    for (const SparseVectorBase *const ptr : m_table | std::views::values) {
      count += ptr->size();
    }
    return count;
  }

  auto empty() const noexcept -> bool { return nTypes() == 0; }

  template <typename T> //
  constexpr auto empty() const noexcept -> bool {
    const SparseVector<T> *ptr = getSparseVectorPtr<T>();
    return ptr == nullptr ? true : ptr->size() == 0;
  }

  template <typename T> //
  constexpr auto exists() const noexcept -> bool {
    return getSparseVectorPtr<T>() != nullptr;
  }

  template <typename T> //
  constexpr auto exists(u64 id) const noexcept -> bool {
    const SparseVector<T> *ptr = getSparseVectorPtr<T>();
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
    SparseVector<T> *ptr =
        static_cast<SparseVector<T> *>(tryEmplace<T>()->second);
    ptr->add(id, v);
  }

  /**
   * @brief <code>haha</code>
   *
   * @tparam T
   * @param id
   * @return T&
   */
  template <typename T> //
  constexpr auto get(u64 id) -> T & {
    SparseVector<T> *ptr = getSparseVectorPtr<T>();
    if (ptr == nullptr) {
      throw std::runtime_error("TypeMap::get()");
    }
    return getSparseVectorPtr<T>()->operator[](id);
  }

  /**
   * {@link #add}
   */
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
    SparseVector<T> *ptr = getSparseVectorPtr<T>();
    if (ptr == nullptr) {
      return;
    }
    ptr->remove(id);
  }

  auto clear() noexcept -> void {
    for (SparseVectorBase *const ptr : m_table | std::views::values) {
      delete ptr;
    }
    m_table.clear();
  }

  template <typename T> //
  constexpr auto clear() noexcept -> void {
    SparseVector<T> *ptr = getSparseVectorPtr<T>();
    if (ptr == nullptr) {
      return;
    }
    ptr->clear();
  }

  auto reset() noexcept -> void {
    for (SparseVectorBase *const ptr : m_table | std::views::values) {
      ptr->clear();
    }
  }

private:
  template <typename T> //
  auto getSparseVectorPtr() noexcept -> SparseVector<T> * {
    auto it = m_table.find(typeid(T));
    if (it == m_table.end()) {
      return nullptr;
    }
    //* This static down cast is safe
    return static_cast<SparseVector<T> *>(it->second);
  }

  template <typename T> //
  auto getSparseVectorPtr() const noexcept -> const SparseVector<T> * {
    auto it = m_table.find(typeid(T));
    if (it == m_table.end()) {
      return nullptr;
    }
    //* This static down cast is safe
    return static_cast<SparseVector<T> *>(it->second);
  }

  template <typename T> //
  auto getEntry() noexcept -> TableType::iterator {
    return m_table.find(typeid(T));
  }

  template <typename T> //
  auto tryEmplace() -> TableType::iterator {
    auto *pSparseVector =
        static_cast<SparseVector<T> *>(m_allocator.resource()->allocate(
            sizeof(SparseVector<T>), alignof(SparseVector<T>)));
    m_allocator.construct(pSparseVector);
    return m_table.try_emplace(typeid(T), pSparseVector).first;
  }

private:
  TableType m_table = {};
  GeneralAllocatorType m_allocator = {};
};

} // namespace tora

#endif // TORA_INCLUDE_TORA_CORE_TYPE_MAP_HPP