#ifndef ECS_SOURCE_BASE_HPP
#define ECS_SOURCE_BASE_HPP

#include <algorithm>
#include <concepts>
#include <memory>
#include <numeric>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Container/DynamicBitset.hpp>

namespace ecs {

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef bool b8;
typedef uint32_t b32;

typedef const char *cString;

#ifndef ECS_ALLOW_EXCEPTIONS // Debug mode
constexpr bool allowExceptions = true;
#else  // Release mode
constexpr bool allowExceptions = false;
#endif // ECS_ALLOW_EXCEPTIONS

#define ECS_NOEXCEPT noexcept(!allowExceptions)

template <typename ExceptionType = std::runtime_error>
void expect(bool condition, cString exceptionMessage) {
  if (!condition) {
    throw ExceptionType(exceptionMessage);
  }
}

template <typename Type> //
cString typenameStr() noexcept {
  return typeid(Type).name();
}

template <u64 Index, typename... Args> //
void iterateTuple(std::tuple<Args...> &tp, auto func) {
  func(std::get<Index>(tp));
  if constexpr (Index + 1 != sizeof...(Args)) {
    iterateTuple<Index + 1>(tp, func);
  }
}

template <typename> //
struct First2ArgTypes;

template <typename FuncType, typename Arg1Type, typename Arg2Type,
          typename... Args> //
struct First2ArgTypes<FuncType(Arg1Type, Arg2Type, Args...)> {
  using type1 = Arg1Type;
  using type2 = Arg2Type;
};

// typedef std::vector<bool> DynamicBitset;
using container::DynamicBitset;

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

  DataType &at(u64 id) ECS_NOEXCEPT {
    if constexpr (allowExceptions) {
      expect(this->exists(id), "SparseVector::operator[]: unknown ID");
    }
    return this->mData[this->mIDToIndex[id]];
  }

  DataType &operator[](u64 id) ECS_NOEXCEPT { return this->at(id); }

  DataType &operator[](u64 id) const ECS_NOEXCEPT { return this->at(id); }

  void add(u64 id, DataType value) {
    if (!this->exists(id)) {
      this->mData.emplace_back(value);
      this->mIDToIndex[id] = this->mData.size() - 1;
      this->mIndexToID[this->mData.size() - 1] = id;
    } else {
      this->mData[this->mIDToIndex[id]] = std::move(value);
    }
  }

  void remove(u64 id) noexcept override {
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

  bool exists(const u64 &id) const noexcept {
    return this->mIDToIndex.find(id) != this->mIDToIndex.end();
  }
};

typedef SparseVector<u64> UniqueIDContainer;

} // namespace ecs

#endif // ECS_SOURCE_BASE_HPP