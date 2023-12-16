#ifndef ECS_BASE_HPP
#define ECS_BASE_HPP

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <cmath>
#include <concepts>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace ecs {

typedef uint64_t u64;
typedef double f64;
typedef const char *cString;

void runtimeAssert(bool condition, const char *exceptionMessage) {
  if (!condition) {
    throw std::runtime_error(exceptionMessage);
  }
}

template <typename Type> //
cString typenameStr() noexcept {
  return typeid(Type).name();
}

template <uint64_t Index, typename... Args> //
void iterateTuple(std::tuple<Args...> &tp, auto func) {
  func(std::get<Index>(tp));
  if constexpr (Index + 1 != sizeof...(Args)) {
    iterateTuple<Index + 1>(tp, func);
  }
}

typedef std::vector<bool> DynamicBitset;

// template <typename DataType>
// using PolymorphicVector = std::vector<std::unique_ptr<DataType>>;

class DBitset {};

class ScopedTimer {
public:
  ScopedTimer() = default;

  ~ScopedTimer() noexcept {
    std::cout << "Time elapsed: "
              << std::chrono::duration<double, std::milli>(
                     std::chrono::high_resolution_clock::now() - mStart)
              << "\n";
  }

private:
  const decltype(std::chrono::high_resolution_clock::now()) mStart =
      std::chrono::high_resolution_clock::now();
};

class ISparseVector {
public:
  virtual ~ISparseVector() = default;
  virtual void remove(u64) = 0;
};

// template <typename DataType, typename IDType = u64> //
template <typename DataType> //
class SparseVector : public ISparseVector {
public:
  u64 size() const noexcept { return this->mData.size(); }

  auto begin() const noexcept { return this->mData.cbegin(); }
  auto end() const noexcept { return this->mData.cend(); }
  // auto cbegin() const noexcept { return this->mData.cbegin(); }
  // auto cend() const noexcept { return this->mData.cend(); }

  DataType &at(u64 id) {
    if (!this->exists(id)) {
      throw std::runtime_error("SparseVector::operator[]: unknown ID");
    }
    return this->mData[this->mIDToIndex[id]];
  }

  DataType &operator[](u64 id) { return this->at(id); }

  DataType &operator[](u64 id) const { return this->at(id); }

  // DataType &operator[](u64 id) {
  //   if (!this->exists(id)) {
  //     throw std::runtime_error("SparseVector::operator[]: unknown ID");
  //   }
  //   auto index = this->mIDToIndex.at(id);
  //   return this->mData[index];
  // }

  void add(u64 id, DataType value) {
    if (!this->exists(id)) {
      this->mData.emplace_back(value);
      this->mIDToIndex[id] = this->mData.size() - 1;
      this->mIndexToID[this->mData.size() - 1] = id;
    } else {
      this->mData[this->mIDToIndex[id]] = std::move(value);
    }
  }

  void remove(u64 id) override {
    if (!this->exists(id)) {
      return;
    }

    auto lastElementIndex = this->mData.size() - 1;
    auto lastElementID = this->mIndexToID[lastElementIndex];
    auto removedIndex = this->mIDToIndex[id];

    this->mIndexToID[removedIndex] = lastElementID;
    this->mIDToIndex[lastElementID] = removedIndex;

    std::swap(this->mData[removedIndex], this->mData[lastElementIndex]);
    this->mIndexToID.erase(lastElementIndex);
    this->mIDToIndex.erase(id);
    this->mData.pop_back();
  }

private:
  std::unordered_map<u64, u64> mIDToIndex = {};
  std::unordered_map<u64, u64> mIndexToID = {};
  std::vector<DataType> mData = {};

  bool exists(const u64 &id) {
    return this->mIDToIndex.find(id) != this->mIDToIndex.end();
  }
};

} // namespace ecs

#endif // ECS_BASE_HPP