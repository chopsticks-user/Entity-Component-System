#ifndef USHI_INCLUDE_USHI_CONTAINER_UNORDERED_DENSE_MAP_HPP
#define USHI_INCLUDE_USHI_CONTAINER_UNORDERED_DENSE_MAP_HPP

#include "Ushi/Core/Core.hpp"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace ushi {

struct UnorderedDenseMapBase {
  virtual ~UnorderedDenseMapBase() = default;
  virtual auto size() const noexcept -> u64 = 0;
  virtual auto remove(u64 id) noexcept -> void = 0;
  virtual auto clear() noexcept -> void = 0;
};

template <typename T_Key, typename T_Value> //
class UnorderedDenseMap : public UnorderedDenseMapBase {
  using T_KeyToIndexMap = std::unordered_map<T_Key, u64>;
  using T_IndexToKeyMap = std::unordered_map<u64, T_Key>;
  using T_ValueStorage = std::vector<T_Value>;

  // TODO: implement an iterator

public:
  constexpr auto operator[](u64 id) -> T_Value & {
    return m_storage[m_keyToIndex.at(id)];
  }

  constexpr auto operator[](u64 id) const -> const T_Value & {
    return m_storage[m_keyToIndex.at(id)];
  }

public:
  auto size() const noexcept -> u64 override { return m_storage.size(); }

  constexpr auto begin() noexcept { return m_storage.begin(); }
  constexpr auto end() noexcept { return m_storage.end(); }

  constexpr auto cbegin() const noexcept { return m_storage.cbegin(); }
  constexpr auto cend() const noexcept { return m_storage.cend(); }

  constexpr auto exists(const u64 &id) const noexcept -> bool {
    return m_keyToIndex.find(id) != m_keyToIndex.end();
  }

  constexpr auto empty() const noexcept -> bool { return m_storage.empty(); }

  constexpr auto at(u64 id) -> T_Value & { return (*this)[id]; }

  constexpr auto at(u64 id) const -> const T_Value & { return (*this)[id]; }

  constexpr auto add(u64 id, T_Value value) -> void {
    if (!exists(id)) {
      m_storage.emplace_back(value);
      m_keyToIndex[id] = m_storage.size() - 1;
      m_indexToKey[m_storage.size() - 1] = m_keyToIndex[id];
    } else {
      m_storage[m_keyToIndex[id]] = std::move(value);
    }
  }

  auto remove(u64 id) noexcept -> void override {
    if (!exists(id)) {
      return;
    }

    auto lastElementIndex = m_storage.size() - 1;
    auto lastElementID = m_indexToKey[lastElementIndex];
    auto removedIndex = m_keyToIndex[id];

    m_indexToKey[removedIndex] = lastElementID;
    m_keyToIndex[lastElementID] = removedIndex;

    std::swap(m_storage[removedIndex], m_storage[lastElementIndex]);
    m_indexToKey.erase(lastElementIndex);
    m_keyToIndex.erase(id);
    m_storage.pop_back();
  }

  auto clear() noexcept -> void override {
    m_storage.clear();
    m_indexToKey.clear();
    m_keyToIndex.clear();
  }

private:
  // If value type is ref or ptr type
  constexpr static auto m_constructIndexToKeyMap(const T_KeyToIndexMap &from)
      -> T_IndexToKeyMap {
    T_IndexToKeyMap map;
    std::ranges::for_each(
        from, [&](const T_Key &key, const u64 &index) { map[index] = key; });
    return map;
  }

private:
  T_KeyToIndexMap m_keyToIndex = {};
  T_IndexToKeyMap m_indexToKey = {};
  T_ValueStorage m_storage = {};
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_CONTAINER_UNORDERED_DENSE_MAP_HPP
