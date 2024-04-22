#ifndef USHI_INCLUDE_USHI_CONTAINER_TYPE_ERASED_VECTOR_HPP
#define USHI_INCLUDE_USHI_CONTAINER_TYPE_ERASED_VECTOR_HPP

#include "Core/Core.hpp"

namespace ushi {
namespace internal {
namespace container {

class TypeErasedVector final {
public:
  constexpr TypeErasedVector() noexcept = default;

  constexpr TypeErasedVector(size_t elementSize) noexcept
      : m_elementSize{elementSize}, m_vector{} {}

public:
  constexpr auto size() const noexcept -> size_t {
    return m_vector.size() / m_elementSize;
  }

  constexpr auto elementSize() const noexcept -> size_t {
    return m_elementSize;
  }

  template <typename T> //
  constexpr auto at(size_t index) -> T & {
    m_throw_if_sizes_mismatch<T>();
    m_throw_if_out_of_bounds(index);
    return *reinterpret_cast<T *>(m_vector.data() + index * m_elementSize);
  }

  template <typename T> //
  constexpr auto at(size_t index) const -> T & {
    m_throw_if_sizes_mismatch<T>();
    m_throw_if_out_of_bounds(index);
    return *reinterpret_cast<T *>(m_vector.data() + index * m_elementSize);
  }

  // TODO: ensure move semantics
  template <typename T> //
  constexpr auto add(T &&value) -> void {
    m_throw_if_sizes_mismatch<T>();
    auto *pBytes = reinterpret_cast<std::byte *>(&value);
    m_vector.insert(m_vector.end(), pBytes, pBytes + m_elementSize);
    // m_vector.emplace(m_vector.end(), pBytes, pBytes + m_elementSize);
  }

  // TODO: ensure move semantics
  constexpr auto merge(TypeErasedVector other) -> void {
    if (other.m_elementSize != m_elementSize) {
      throw;
    }
    m_vector.insert(m_vector.end(),
                    std::make_move_iterator(other.m_vector.begin()),
                    std::make_move_iterator(other.m_vector.end()));
  }

  template <typename T> //
  constexpr auto replace(size_t index, T &&newValue) -> void {
    m_throw_if_sizes_mismatch<T>();
    m_throw_if_out_of_bounds(index);
    *reinterpret_cast<std::remove_cvref_t<T> *>(
        m_vector.data() + index * m_elementSize) = std::forward<T>(newValue);
  }

  template <typename T> //
  [[nodiscard]] constexpr auto remove(size_t index) -> T {
    m_throw_if_sizes_mismatch<T>();
    if (m_out_of_bounds(index)) {
      return {};
    }

    auto rmBegin = m_vector.begin() + index * m_elementSize;
    auto vecEnd = m_vector.end();
    std::swap_ranges(rmBegin, rmBegin + m_elementSize, vecEnd - m_elementSize);

    T rmValue = std::move(*reinterpret_cast<T *>(
        m_vector.data() + m_vector.size() - m_elementSize));
    m_vector.erase(vecEnd - m_elementSize, vecEnd);
    return rmValue;
  }

  constexpr auto clear() noexcept -> void { m_vector.clear(); }

private:
  template <typename T> //
  constexpr auto m_throw_if_sizes_mismatch() const -> void {
    if (sizeof(std::remove_cvref_t<T>) != m_elementSize) {
      throw std::runtime_error("m_throw_if_sizes_mismatch");
    }
  }

  constexpr auto m_throw_if_out_of_bounds(size_t index) const -> void {
    if (m_out_of_bounds(index)) {
      throw std::runtime_error("m_throw_if_out_of_bounds");
    }
  }

  constexpr auto m_out_of_bounds(size_t index) const -> bool {
    return !(index * m_elementSize < m_vector.size());
  }

private:
  size_t m_elementSize = 1;
  std::vector<std::byte> m_vector = {};
};

} // namespace container
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_CONTAINER_TYPE_ERASED_VECTOR_HPP
