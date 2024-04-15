#ifndef USHI_INCLUDE_USHI_INTERFACE_PUBLIC_HPP
#define USHI_INCLUDE_USHI_INTERFACE_PUBLIC_HPP

#include "Ushi/Container/UnorderedDenseMap.hpp"

#include "Ushi/Impl/Component/Component.hpp"
#include "Ushi/Impl/Config/Config.hpp"
#include "Ushi/Impl/Entity/Entity.hpp"
#include "Ushi/Impl/System/System.hpp"
#include "Ushi/Impl/World/World.hpp"

namespace ushi {

namespace container {
using internal::container::UnorderedDenseMap;
} // namespace container

namespace memory {} // namespace memory

//================================Concepts=====================================

using internal::impl::IsComponent;
using internal::impl::IsConfig;
using internal::impl::IsEID;
using internal::impl::IsEIDGenerator;
using internal::impl::IsSignature;

//================================Typenames====================================

using internal::impl::Component;
using internal::impl::DefaultConfig;
using internal::impl::Entity;
using internal::impl::World;

} // namespace ushi

#endif // USHI_INCLUDE_USHI_INTERFACE_PUBLIC_HPP
