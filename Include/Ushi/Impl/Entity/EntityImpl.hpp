#ifndef TORA_INCLUDE_TORA_IMPL_ENTITY_IMPL_HPP
#define TORA_INCLUDE_TORA_IMPL_ENTITY_IMPL_HPP

#include "Ushi/Core/Core.hpp"

#include "EntityID.hpp"
#include "Ushi/Impl/Config/Config.hpp"

namespace ushi {

class EntityFactory;

/**
 * Inheriting from this class is currently not supported.
 *
 * @tparam T_Config
 */
template <typename T_Config> //
class Entity final {
  friend class EntityFactory;

  using T_Signature = T_Config::SignatureType;

public:
  virtual ~Entity() noexcept = default;

public:
  constexpr auto id() const noexcept -> EntityID { return m_id; }

  constexpr auto signature() const noexcept -> const T_Signature & {
    return m_signature;
  }

  static constexpr auto maxComponents() noexcept -> u64 {
    return T_Signature{}.size();
  }

  constexpr operator EntityID() const noexcept { return m_id; }

private:
  constexpr Entity(const EntityID &id, T_Signature signature) noexcept
      : m_id{id}, m_signature{std::move(signature)} {}

private:
  const EntityID m_id;
  T_Signature m_signature;
};

template <class T_Entity>
concept IsValidEntity =
    IsValidConfig<typename FirstTemplateArg<T_Entity>::Type>;

} // namespace ushi

#endif // TORA_INCLUDE_TORA_IMPL_ENTITY_IMPL_HPP
