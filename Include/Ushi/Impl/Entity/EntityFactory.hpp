#ifndef USHI_INCLUDE_USHI_IMPL_ENTITY_FACTORY_HPP
#define USHI_INCLUDE_USHI_IMPL_ENTITY_FACTORY_HPP

#include "Ushi/Core/Core.hpp"

#include "Config/Config.hpp"
#include "EntityID.hpp"
#include "EntityImpl.hpp"

namespace ushi {
namespace internal {
namespace impl {

template <IsEntity T_Entity>
using T_EntitySignature = core::FirstTemplateArg<T_Entity>::Type::SignatureType;

template <IsEIDGenerator T_EIDGenerator> //
class EntityFactory {
public:
  template <IsEntity T_Entity>
  constexpr auto
  create(T_EntitySignature<T_Entity> entitySignature = {}) noexcept
      -> T_Entity {
    return T_Entity{m_idGenerator(), std::move(entitySignature)};
  }

  template <IsConfig T_Config>
  constexpr auto create(T_Config::SignatureType entitySignature = {}) noexcept
      -> Entity<T_Config> {
    return Entity<T_Config>{m_idGenerator(), std::move(entitySignature)};
  }

  template <IsEntity T_Entity>
  constexpr auto clone(const T_Entity &other) noexcept -> T_Entity {
    return T_Entity{m_idGenerator(), other.m_signature};
  }

  template <IsEntity T_Entity>
  constexpr auto setSignature(T_Entity &entity,
                              T_EntitySignature<T_Entity> signature) noexcept
      -> void {
    entity.m_signature = std::move(signature);
  }

  template <IsEntity T_Entity>
  constexpr auto setSignatureBit(T_Entity &entity, u64 bitIndex,
                                 bool bitValue) noexcept -> void {
    entity.m_signature[bitIndex] = bitValue;
  }

private:
  T_EIDGenerator m_idGenerator = {};
};

} // namespace impl
} // namespace internal
} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_ENTITY_FACTORY_HPP
