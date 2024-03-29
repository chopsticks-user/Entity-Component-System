#ifndef TORA_INCLUDE_TORA_IMPL_ENTITY_IMPL_HPP
#define TORA_INCLUDE_TORA_IMPL_ENTITY_IMPL_HPP

#include "Ushi/Core/Core.hpp"

#include "EntityID.hpp"
#include "Ushi/Impl/Config/Config.hpp"

namespace ushi {

template <IsEIDGenerator> class EntityFactory;

/**
 * Inheriting from this class is currently not supported.
 *
 * @tparam T_Config
 */
template <IsConfig T_Config> //
class Entity final {
  friend class EntityFactory<typename T_Config::EIDGeneratorType>;

  using T_ID = FirstTemplateArg<typename T_Config::EIDGeneratorType>::Type;
  using T_Signature = T_Config::SignatureType;

public:
  constexpr Entity(const Entity &) noexcept = default;

  constexpr Entity(Entity &&) noexcept = default;

  constexpr auto operator=(const Entity &) noexcept -> Entity & = delete;

  constexpr auto operator=(Entity &&other) noexcept -> Entity & {
    m_id = std::move(other.m_id);
    m_signature = std::move(other.m_signature);
    return *this;
  };

  virtual ~Entity() noexcept = default;

public:
  constexpr auto id() const noexcept -> T_ID { return m_id; }

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
  T_ID m_id;
  T_Signature m_signature;
};

template <class T_Entity>
concept IsEntity = IsConfig<typename FirstTemplateArg<T_Entity>::Type>;

} // namespace ushi

// template <ushi::IsEntity T_Entity> struct std::hash<T_Entity> {
//   std::size_t operator()(const ushi::EntityID &entityID) const noexcept {
//     return std::hash<ushi::EntityID>{}(entityID);
//   }
// };

#endif // TORA_INCLUDE_TORA_IMPL_ENTITY_IMPL_HPP
