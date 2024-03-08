#ifndef TORA_INCLUDE_TORA_CORE_SPARSE_VECTOR_HPP
#define TORA_INCLUDE_TORA_CORE_SPARSE_VECTOR_HPP

#include "Forward.hpp"

#include <unordered_map>
#include <vector>

namespace tora {

template <typename DataType, typename IndexType = u64> //
class SparseVector : public SparseVectorBase {
public:
  auto operator[](u64 id) -> DataType & {
    return this->m_data[this->m_idToIndex.at(id)];
  }

  auto operator[](u64 id) const -> DataType & {
    return this->m_data[this->m_idToIndex.at(id)];
  }

public:
  auto size() const noexcept -> u64 { return this->m_data.size(); }

  auto begin() const noexcept { return this->m_data.cbegin(); }
  auto end() const noexcept { return this->m_data.cend(); }

  auto exists(const u64 &id) const noexcept -> bool {
    return this->m_idToIndex.find(id) != this->m_idToIndex.end();
  }

  DataType &at(u64 id) { return (*this)[id]; }

  void add(u64 id, DataType value) {
    if (!this->exists(id)) {
      this->m_data.emplace_back(value);
      this->m_idToIndex[id] = this->m_data.size() - 1;
      this->mIndexToID[this->m_data.size() - 1] = id;
    } else {
      this->m_data[this->m_idToIndex[id]] = std::move(value);
    }
  }

  void remove(u64 id) noexcept override {
    if (!this->exists(id)) {
      return;
    }

    auto lastElementIndex = this->m_data.size() - 1;
    auto lastElementID = this->mIndexToID[lastElementIndex];
    auto removedIndex = this->m_idToIndex[id];

    this->mIndexToID[removedIndex] = lastElementID;
    this->m_idToIndex[lastElementID] = removedIndex;

    std::swap(this->m_data[removedIndex], this->m_data[lastElementIndex]);
    this->mIndexToID.erase(lastElementIndex);
    this->m_idToIndex.erase(id);
    this->m_data.pop_back();
  }

private:
  std::unordered_map<u64, u64> m_idToIndex = {};
  std::unordered_map<u64, u64> mIndexToID = {};
  std::vector<DataType> m_data = {};
};

} // namespace tora

#endif // TORA_INCLUDE_TORA_CORE_SPARSE_VECTOR_HPP