#ifndef USHI_INCLUDE_USHI_CONTAINER_UNORDERED_DENSE_MAP_HPP
#define USHI_INCLUDE_USHI_CONTAINER_UNORDERED_DENSE_MAP_HPP

#include "Core/Core.hpp"

namespace ushi {
namespace internal {
namespace container {

template <typename T_Key> //
struct UnorderedDenseMapBase {
  virtual ~UnorderedDenseMapBase() = default;
  virtual auto size() const noexcept -> u64 = 0;
  virtual auto remove(const T_Key &id) noexcept -> void = 0;
  virtual auto clear() noexcept -> void = 0;
};

template <typename T_Key, typename T_Value> //
class UnorderedDenseMap : public UnorderedDenseMapBase<T_Key> {
  using T_KeyToIndexMap = std::unordered_map<T_Key, u64>;
  using T_IndexToKeyMap = std::unordered_map<u64, T_Key>;
  using T_ValueStorage = std::vector<T_Value>;

  // TODO: implement an iterator

public:
  constexpr auto operator[](const T_Key &id) -> T_Value & {
    return m_storage[m_keyToIndex.at(id)];
  }

  constexpr auto operator[](const T_Key &id) const -> const T_Value & {
    return m_storage[m_keyToIndex.at(id)];
  }

public:
  auto size() const noexcept -> u64 override { return m_storage.size(); }

  constexpr auto begin() noexcept { return m_storage.begin(); }
  constexpr auto end() noexcept { return m_storage.end(); }

  constexpr auto cbegin() const noexcept { return m_storage.cbegin(); }
  constexpr auto cend() const noexcept { return m_storage.cend(); }

  constexpr auto contains(const T_Key &id) const noexcept -> bool {
    return m_keyToIndex.find(id) != m_keyToIndex.end();
  }

  constexpr auto empty() const noexcept -> bool { return m_storage.empty(); }

  constexpr auto at(const T_Key &id) -> T_Value & { return (*this)[id]; }

  constexpr auto at(const T_Key &id) const -> const T_Value & {
    return (*this)[id];
  }

  constexpr auto indexOf(const T_Key &id) const noexcept -> u64 {
    return m_keyToIndex.at(id);
  }

  constexpr auto getKeys() const noexcept -> std::vector<T_Key> {
    std::vector<T_Key> keys{};
    keys.reserve(size());
    for (const auto &p : m_keyToIndex) {
      keys.push_back(p.first);
    }
    return keys;
  }

  constexpr auto add(const T_Key &id, T_Value value) -> void {
    if (!contains(id)) {
      m_storage.emplace_back(value);
      m_keyToIndex[id] = m_storage.size() - 1;
      m_indexToKey[m_storage.size() - 1] = m_keyToIndex[id];
    } else {
      m_storage[m_keyToIndex[id]] = std::move(value);
    }
  }

  auto remove(const T_Key &id) noexcept -> void override {
    if (!contains(id)) {
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

} // namespace container
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_CONTAINER_UNORDERED_DENSE_MAP_HPP
