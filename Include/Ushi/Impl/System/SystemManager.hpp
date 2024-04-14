#ifndef USHI_INCLUDE_USHI_IMPL_SYSTEM_MANAGER_HPP
#define USHI_INCLUDE_USHI_IMPL_SYSTEM_MANAGER_HPP

#include "Core/Core.hpp"

#include "Component/Component.hpp"
#include "Config/Config.hpp"
#include "Entity/Entity.hpp"

namespace ushi {

template <IsConfig T_Config> //
class SystemManager final {
  using T_Signature = T_Config::SignatureType;
  using T_SystemTable = std::unordered_map<std::type_index, T_Signature>;
  using T_Archetype = std::unordered_map<T_Signature, std::vector<EntityID>>;

public:
  template <typename T_System, IsComponent... T_Components> //
  constexpr auto regster(const ComponentTable<T_Config> &componentTable)
      -> void {
    auto tpIndex = std::type_index{typeid(T_System)};
    if (m_systemTable.contains(tpIndex)) {
      return;
    }

    T_Signature systemSignature =
        componentTable.template signature<T_Components...>();
    m_systemTable[tpIndex] = systemSignature;
    if (m_archetype.contains(systemSignature)) {
      return;
    }
    m_archetype[systemSignature] =
        componentTable.template allEntitiesWith<T_Components...>();
  }

  template <typename T_System> //
  constexpr auto execute(ComponentTable<T_Config> &componentTable) const
      -> void {
    typename ArgumentTuple<decltype(T_System::function)>::Type sampleTuple{};

    for (const auto &entityID :
         m_archetype.at(m_systemTable.at(typeid(T_System)))) {
      auto args = componentTable.template componentsOf(entityID, sampleTuple);
      std::apply(T_System::function, args);
    }
  }

private:
  T_SystemTable m_systemTable;
  T_Archetype m_archetype;
};

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_SYSTEM_MANAGER_HPP