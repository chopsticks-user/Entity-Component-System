#ifndef USHI_INCLUDE_USHI_IMPL_ENTITY_FACTORY_HPP
#define USHI_INCLUDE_USHI_IMPL_ENTITY_FACTORY_HPP

#include "Ushi/Core/Core.hpp"

#include "EntityID.hpp"
#include "EntityImpl.hpp"
#include "Ushi/Impl/Config/Config.hpp"

namespace ushi {

template <IsEntity T_Entity>
using T_EntitySignature = FirstTemplateArg<T_Entity>::Type::SignatureType;

/**
 * @brief
 *
 */
template <IsEIDGenerator T_EIDGenerator> //
class EntityFactory {
public:
  /**
   * @brief
   *
   * @tparam T_Entity
   * @param entitySignature
   * @return T_Entity
   */
  template <IsEntity T_Entity>
  constexpr auto
  create(T_EntitySignature<T_Entity> entitySignature = {}) noexcept
      -> T_Entity {
    return T_Entity{m_idGenerator(), std::move(entitySignature)};
  }

  /**
   * @brief
   *
   * @tparam T_Config
   * @param entitySignature
   * @return Entity<T_Config>
   */
  template <IsConfig T_Config>
  constexpr auto create(T_Config::SignatureType entitySignature = {}) noexcept
      -> Entity<T_Config> {
    return Entity<T_Config>{m_idGenerator(), std::move(entitySignature)};
  }

  /**
   * @brief
   *
   * @tparam T_Entity
   * @param other
   * @return T_Entity
   */
  template <IsEntity T_Entity>
  constexpr auto clone(const T_Entity &other) noexcept -> T_Entity {
    return T_Entity{m_idGenerator(), other.m_signature};
  }

  /**
   * @brief Set the Signature object
   *
   * @tparam T_Entity
   * @param entity
   * @param signature
   */
  template <IsEntity T_Entity>
  constexpr auto setSignature(T_Entity &entity,
                              T_EntitySignature<T_Entity> signature) noexcept
      -> void {
    entity.m_signature = std::move(signature);
  }

  /**
   * Undefined if bitIndex >= m_signature.size()
   *
   * @param bitIndex
   * @param bitValue
   */
  template <IsEntity T_Entity>
  constexpr auto setSignatureBit(T_Entity &entity, u64 bitIndex,
                                 bool bitValue) noexcept -> void {
    entity.m_signature[bitIndex] = bitValue;
  }

private:
  T_EIDGenerator m_idGenerator = {};
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_ENTITY_FACTORY_HPP
