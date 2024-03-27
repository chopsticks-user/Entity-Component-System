#ifndef TORA_INCLUDE_TORA_CORE_SPARSE_VECTOR_HPP
#define TORA_INCLUDE_TORA_CORE_SPARSE_VECTOR_HPP

#include "../Core/Forward.hpp"

#include <unordered_map>
#include <vector>

namespace tora {

struct SparseVectorBase {
  virtual ~SparseVectorBase() = default;
  virtual auto size() const noexcept -> u64 = 0;
  virtual auto remove(u64 id) noexcept -> void = 0;
  virtual auto clear() noexcept -> void = 0;
};

template <typename TData> //
class SparseVector : public SparseVectorBase {
  using UMapType = std::pmr::unordered_map<u64, u64>;
  using ContainerType = std::pmr::vector<TData>;

  // TODO: implement an iterator

public:
  constexpr auto operator[](u64 id) -> TData & {
    return m_data[m_idToIndex.at(id)];
  }

  constexpr auto operator[](u64 id) const -> const TData & {
    return m_data[m_idToIndex.at(id)];
  }

public:
  auto size() const noexcept -> u64 override { return m_data.size(); }

  constexpr auto begin() const noexcept { return m_data.cbegin(); }
  constexpr auto end() const noexcept { return m_data.cend(); }

  constexpr auto exists(const u64 &id) const noexcept -> bool {
    return m_idToIndex.find(id) != m_idToIndex.end();
  }

  constexpr auto empty() const noexcept -> bool { return m_data.empty(); }

  constexpr auto at(u64 id) -> TData & { return (*this)[id]; }

  constexpr auto at(u64 id) const -> const TData & { return (*this)[id]; }

  constexpr auto add(u64 id, TData value) -> void {
    if (!exists(id)) {
      m_data.emplace_back(value);
      m_idToIndex[id] = m_data.size() - 1;
      m_indexToID[m_data.size() - 1] = id;
    } else {
      m_data[m_idToIndex[id]] = std::move(value);
    }
  }

  auto remove(u64 id) noexcept -> void override {
    if (!exists(id)) {
      return;
    }

    auto lastElementIndex = m_data.size() - 1;
    auto lastElementID = m_indexToID[lastElementIndex];
    auto removedIndex = m_idToIndex[id];

    m_indexToID[removedIndex] = lastElementID;
    m_idToIndex[lastElementID] = removedIndex;

    std::swap(m_data[removedIndex], m_data[lastElementIndex]);
    m_indexToID.erase(lastElementIndex);
    m_idToIndex.erase(id);
    m_data.pop_back();
  }

  auto clear() noexcept -> void {
    m_data.clear();
    m_indexToID.clear();
    m_idToIndex.clear();
  }

private:
  UMapType m_idToIndex = {};
  UMapType m_indexToID = {};
  ContainerType m_data = {};
};

} // namespace tora

#endif // TORA_INCLUDE_TORA_CORE_SPARSE_VECTOR_HPP
