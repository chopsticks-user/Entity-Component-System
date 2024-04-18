#ifndef USHI_INCLUDE_USHI_CONTAINER_UDENSE_TYPE_TABLE_HPP
#define USHI_INCLUDE_USHI_CONTAINER_UDENSE_TYPE_TABLE_HPP

#include "Core/Core.hpp"

namespace ushi {
namespace internal {
namespace container {

template <typename TValueBase> //
struct VectorWrapperBase {
  virtual ~VectorWrapperBase() = default;

  virtual auto size() const noexcept -> u64 = 0;

  virtual auto remove(u64 index) noexcept -> void = 0;

  // TODO:
  virtual auto push(void *pValue) -> void = 0;

  virtual auto popAt(u64 index) noexcept -> TValueBase * = 0;

  virtual auto replaceAt(u64 index, void *pValue) noexcept -> void = 0;

  virtual auto clear() noexcept -> void = 0;
};

template <typename TValue, typename TValueBase>
  requires core::IsChildOf<TValue, TValueBase>
class VectorWrapper : public VectorWrapperBase<TValueBase> {
public:
  constexpr auto get() noexcept -> std::vector<TValue> & { return m_vector; }

  virtual auto push(void *pValue) -> void override {
    m_vector.push_back(std::move(*static_cast<TValue *>(pValue)));
  }

  // * Return a raw ptr
  [[nodiscard]] virtual auto popAt(u64 index) noexcept -> TValue * override {
    std::swap(m_vector[index], m_vector.back());
    TValue *ptr = new TValue{};
    *ptr = std::move(m_vector.back());
    m_vector.pop_back();
    return ptr;
  }

  virtual auto replaceAt(u64 index, void *pValue) noexcept -> void {
    m_vector[index] = std::move(*static_cast<TValue *>(pValue));
  }

  [[nodiscard]] virtual auto size() const noexcept -> u64 {
    return m_vector.size();
  }

  virtual auto remove(u64 index) noexcept -> void override {
    std::swap(m_vector[index], m_vector.back());
    m_vector.pop_back();
  }

  virtual auto clear() noexcept -> void override { m_vector.clear(); }

private:
  std::vector<TValue> m_vector;
};

template <typename TKey, typename TValueBase> //
class PolyTypeTable {
  using TKeyToIndex = std::unordered_map<TKey, u64>;
  using TKeyContainer = std::vector<TKey>;
  using TTable =
      std::unordered_map<std::type_index,
                         std::shared_ptr<VectorWrapperBase<TValueBase>>>;
  using TPackage =
      std::unordered_map<std::type_index, std::unique_ptr<TValueBase>>;

  // TODO: implement an iterator

public:
  // template <typename... TValue> //
  // constexpr auto init() -> void {
  //   if (!m_typeTable.empty()) {
  //     return;
  //   }
  //   (m_initializeType<TValue>(), ...);
  //   m_nTypes = m_typeTable.size(); // * For testing purposes
  // }

  constexpr PolyTypeTable(const TTable &table) noexcept
      : m_keyToIndex{}, m_keyContainer{}, m_typeTable{table} {
    // for (const auto &pair : table) {
    //   m_typeTable[pair.first] = pair.second;
    // }
    m_nTypes = m_typeTable.size();
  }

public:
  [[nodiscard]] auto nTypes() const noexcept -> u64 { return m_nTypes; }

  [[nodiscard]] auto size() const noexcept -> u64 {
    return m_keyContainer.size();
  }

  [[nodiscard]] constexpr auto contains(const TKey &key) const noexcept
      -> bool {
    return m_keyToIndex.find(key) != m_keyToIndex.end();
  }

  template <typename TValue> //
  [[nodiscard]] constexpr auto at(const TKey &key) -> TValue & {
    return m_rowPtr<TValue>()->get()[m_keyToIndex.at(key)];
  }

  template <typename TValue> //
  [[nodiscard]] constexpr auto at(const TKey &key) const -> const TValue & {
    return m_rowPtr<TValue>()->get()[m_keyToIndex.at(key)];
  }

  [[nodiscard]] constexpr auto getKeys() const noexcept
      -> const std::vector<TKey> & {
    return m_keyContainer;
  }

  template <core::IsChildOf<TValueBase>... TValues> //
  constexpr auto addWithTypes(const TKey &key, TValues... values) -> void {
    auto it = m_keyToIndex.find(key);
    if (it != m_keyToIndex.end()) {
      (m_replaceAt<TValues>(it->second, std::move(values)), ...);
      return;
    }

    // * Internal exception
    if (std::tuple_size_v<std::tuple<TValues...>> != nTypes()) {
      throw std::runtime_error(
          "The number of argument types is less than nTypes()");
    }

    m_pushKey(key);
    (std::static_pointer_cast<VectorWrapper<TValues, TValueBase>>(
         m_typeTable.at(typeid(TValues)))
         ->get()
         .push_back(std::move(values)),
     ...);
  }

  template <core::IsChildOf<TValueBase>... TValues> //
  constexpr auto addWithTypes(const TKey &key,
                              std::tuple<TValues...> values = {}) -> void {
    std::apply([&](auto &&...args) { addWithTypes(key, args...); },
               std::move(values));
  }

  // * values.size() == nTypes() and each std::type_index is compatible
  constexpr auto addWithPackage(
      const TKey &key,
      std::unordered_map<std::type_index, std::unique_ptr<TValueBase>> &package)
      -> void {
    // * Internal exception
    if (package.size() < nTypes()) {
      throw std::runtime_error("The number of compatible argument types in "
                               "package is less than nTypes()");
    }

    // * Internal exception
    if (m_keyToIndex.contains(key)) {
      m_replaceAllAt(m_keyToIndex[key], package);
      return;
    }

    m_pushKey(key);
    for (auto &pair : package) {
      auto ptr = pair.second.get();
      m_typeTable[pair.first]->push(ptr);
      pair.second.reset();
    }
  }

  auto remove(const TKey &key) noexcept -> void {
    if (!m_keyToIndex.contains(key)) {
      return;
    }

    u64 removedIndex = m_popKey(key);
    std::ranges::for_each(m_typeTable,
                          [=](auto &p) { p.second->remove(removedIndex); });
  }

  [[nodiscard]] auto popAt(const TKey &key) -> TPackage {
    // * Internal exception
    if (!m_keyToIndex.contains(key)) {
      throw std::runtime_error("popAt(key) requires an existing key");
    }

    u64 removedIndex = m_popKey(key);
    return m_popAtIndex(removedIndex);
  }

  // * For testing purposes only
  [[nodiscard]] constexpr auto validate() -> bool {
    if (m_nTypes != m_typeTable.size()) {
      return false;
    }
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
  template <core::IsChildOf<TValueBase> TValue> //
  constexpr auto m_initializeType() -> void {
    m_typeTable[typeid(TValue)] =
        std::make_shared<VectorWrapper<TValue, TValueBase>>();
  }

  template <core::IsChildOf<TValueBase> TValue> //
  [[nodiscard]] constexpr auto m_rowPtr()
      -> std::shared_ptr<VectorWrapper<TValue, TValueBase>> {
    return std::static_pointer_cast<VectorWrapper<TValue, TValueBase>>(
        m_typeTable.at(typeid(TValue)));
  }

  template <core::IsChildOf<TValueBase> TValue> //
  constexpr auto m_replaceAt(u64 index, TValue value) -> void {
    std::static_pointer_cast<VectorWrapper<TValue, TValueBase>>(
        m_typeTable.at(typeid(TValue)))
        ->get()[index] = std::move(value);
  }

  // * package must contain compatible types
  constexpr auto m_replaceAllAt(u64 index, TPackage &package) -> void {
    for (auto &pair : package) {
      auto ptr = pair.second.get();
      m_typeTable[pair.first]->replaceAt(index, ptr);
      pair.second.reset();
    }
  }

  [[nodiscard]] constexpr auto m_popAtIndex(u64 index) -> TPackage {
    TPackage result;
    for (auto pair : m_typeTable) {
      result[pair.first] =
          std::unique_ptr<TValueBase>(pair.second->popAt(index));
    }
    return result;
  }

  constexpr auto m_pushKey(const TKey &key) noexcept -> void {
    m_keyContainer.push_back(key);
    m_keyToIndex[key] = m_keyContainer.size() - 1;
  }

  // * Do bound checking before calling this function
  constexpr auto m_popKey(const TKey &key) noexcept -> u64 {
    u64 removedIndex = m_keyToIndex[key];
    std::swap(m_keyContainer[removedIndex], m_keyContainer.back());
    m_keyToIndex.at(m_keyContainer[removedIndex]) = removedIndex;
    m_keyContainer.pop_back();
    m_keyToIndex.erase(key);
    return removedIndex;
  }

private:
  TKeyToIndex m_keyToIndex = {};
  TKeyContainer m_keyContainer = {};
  TTable m_typeTable = {};
  u64 m_nTypes = 0; // * For testing purposes
};

} // namespace container
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_CONTAINER_UDENSE_TYPE_TABLE_HPP
