#ifndef USHI_INCLUDE_USHI_CONTAINER_UDENSE_TYPE_TABLE_HPP
#define USHI_INCLUDE_USHI_CONTAINER_UDENSE_TYPE_TABLE_HPP

#include "Core/Core.hpp"

namespace ushi {
namespace internal {
namespace container {

struct VectorWrapperBase {
  virtual ~VectorWrapperBase() = default;

  virtual auto size() const noexcept -> u64 = 0;

  virtual auto remove(u64 index) noexcept -> void = 0;

  virtual auto clear() noexcept -> void = 0;
};

template <typename TValue> //
class VectorWrapper : public VectorWrapperBase {
public:
  constexpr auto get() noexcept -> std::vector<TValue> & { return m_vector; }

  constexpr auto pop(u64 index) noexcept -> TValue {
    std::swap(m_vector[index], m_vector.back());
    TValue value = std::move(m_vector.back());
    m_vector.pop_back();
    return value;
  }

  virtual auto size() const noexcept -> u64 { return m_vector.size(); }

  virtual auto remove(u64 index) noexcept -> void override {
    std::swap(m_vector[index], m_vector.back());
    m_vector.pop_back();
  }

  virtual auto clear() noexcept -> void override { m_vector.clear(); }

private:
  std::vector<TValue> m_vector;
};

template <typename TKey> //
class UDenseTypeTable {
  using TKeyToIndex = std::unordered_map<TKey, u64>;
  using TKeyContainer = std::vector<TKey>;
  using TTable =
      std::unordered_map<std::type_index, std::shared_ptr<VectorWrapperBase>>;

  // TODO: implement an iterator

public:
  template <typename... TValue> //
  constexpr auto init() -> void {
    if (!m_typeTable.empty()) {
      return;
    }
    (m_initializeType<TValue>(), ...);
  }

public:
  auto nTypes() const noexcept -> u64 { return m_typeTable.size(); }

  auto size() const noexcept -> u64 { return m_keyContainer.size(); }

  constexpr auto contains(const TKey &key) const noexcept -> bool {
    return m_keyToIndex.find(key) != m_keyToIndex.end();
  }

  template <typename TValue> //
  constexpr auto at(const TKey &key) -> TValue & {
    return m_rowPtr<TValue>()->get()[m_keyToIndex.at(key)];
  }

  template <typename TValue> //
  constexpr auto at(const TKey &key) const -> const TValue & {
    return m_rowPtr<TValue>()->get()[m_keyToIndex.at(key)];
  }

  constexpr auto getKeys() const noexcept -> const std::vector<TKey> & {
    return m_keyContainer;
  }

  template <typename... TValues> //
  constexpr auto add(const TKey &key, TValues... values) -> void {
    auto it = m_keyToIndex.find(key);

    if (it != m_keyToIndex.end()) {
      (m_replaceAt<TValues>(it->second, std::move(values)), ...);
      return;
    }

    m_keyContainer.push_back(key);
    m_keyToIndex[key] = m_keyContainer.size() - 1;

    (std::static_pointer_cast<VectorWrapper<TValues>>(
         m_typeTable.at(typeid(TValues)))
         ->get()
         .push_back(std::move(values)),
     ...);
  }

  template <typename... TValues> //
  constexpr auto add(const TKey &key, std::tuple<TValues...> values = {})
      -> void {
    std::apply([&](auto &&...args) { add(key, args...); }, std::move(values));
  }

  auto remove(const TKey &key) noexcept -> void {
    auto it = m_keyToIndex.find(key);
    if (it == m_keyToIndex.end()) {
      return;
    }

    u64 removedIndex = it->second;
    std::swap(m_keyContainer[removedIndex], m_keyContainer.back());
    m_keyToIndex.at(m_keyContainer[removedIndex]) = removedIndex;
    m_keyContainer.pop_back();
    m_keyToIndex.erase(it);

    std::ranges::for_each(m_typeTable,
                          [=](auto &p) { p.second->remove(removedIndex); });
  }

  template <typename... TValues> //
  auto pop(const TKey &key) -> std::tuple<TValues...> {
    // TODO: Handle the case when the number of types in TValues... is less than
    // nTypes()
    auto it = m_keyToIndex.find(key);
    if (it == m_keyToIndex.end()) {
      throw std::runtime_error("Key not found");
    }

    u64 removedIndex = it->second;
    auto poppedValues = std::make_tuple(m_popAtIndex<TValues>(removedIndex)...);
    if (std::tuple_size_v<decltype(poppedValues)> < nTypes()) {
      for (auto p : m_typeTable) {
        if (p.second->size() == size()) {
          p.second->remove(removedIndex);
        }
      }
    }

    std::swap(m_keyContainer[removedIndex], m_keyContainer.back());
    m_keyToIndex.at(m_keyContainer[removedIndex]) = removedIndex;
    m_keyContainer.pop_back();
    m_keyToIndex.erase(it);

    return poppedValues;
  }

  // For testing purposes only
  constexpr auto validate() -> bool {
    for (auto p : m_typeTable) {
      if (p.second->size() != size()) {
        return false;
      }
    }
    return true;
  }

  constexpr auto clear() noexcept -> void {
    m_keyContainer.clear();
    m_keyToIndex.clear();
    std::ranges::for_each(m_typeTable, [](auto &p) { p.second->clear(); });
  }

private:
  template <typename TValue> //
  constexpr auto m_initializeType() {
    m_typeTable[typeid(TValue)] = std::make_shared<VectorWrapper<TValue>>();
  }

  template <typename TValue> //
  constexpr auto m_rowPtr() -> std::shared_ptr<VectorWrapper<TValue>> {
    return std::static_pointer_cast<VectorWrapper<TValue>>(
        m_typeTable.at(typeid(TValue)));
  }

  template <typename TValue> //
  constexpr auto m_replaceAt(u64 index, TValue value) -> void {
    std::static_pointer_cast<VectorWrapper<TValue>>(
        m_typeTable.at(typeid(TValue)))
        ->get()[index] = std::move(value);
  }

  template <typename TValue> //
  constexpr auto m_popAtIndex(u64 index) -> TValue {
    return m_typeTable.find(typeid(TValue)) == m_typeTable.end()
               ? TValue{}
               : std::static_pointer_cast<VectorWrapper<TValue>>(
                     m_typeTable.at(typeid(TValue)))
                     ->pop(index);
  }

private:
  TKeyToIndex m_keyToIndex = {};
  TKeyContainer m_keyContainer = {};
  TTable m_typeTable = {};
};

} // namespace container
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_CONTAINER_UDENSE_TYPE_TABLE_HPP
