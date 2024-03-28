#ifndef USHI_INCLUDE_USHI_IMPL_ENTITY_FACTORY_HPP
#define USHI_INCLUDE_USHI_IMPL_ENTITY_FACTORY_HPP

#include "Ushi/Core/Core.hpp"

#include "EntityID.hpp"
#include "EntityImpl.hpp"
#include "Ushi/Impl/Config/Config.hpp"

namespace ushi {

/**
 * @brief
 *
 */
class EntityFactory {
public:
  /**
   * @brief
   *
   * @tparam T_Entity
   * @param entitySignature
   * @return T_Entity
   */
  template <class T_Entity>
    requires IsValidEntity<T_Entity>
  constexpr auto create(T_Entity::T_Signature entitySignature = {}) noexcept
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
  template <typename T_Config>
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
  template <class T_Entity>
    requires IsValidEntity<T_Entity>
  constexpr auto clone(const T_Entity &other) noexcept -> T_Entity {
    return T_Entity{m_idGenerator(), other.m_signature};
  }

private:
  EntityIDGenerator m_idGenerator = {};
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_ENTITY_FACTORY_HPP
