#ifndef USHI_TEST_TEST_UTILS_HPP
#define USHI_TEST_TEST_UTILS_HPP

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

struct CustomConfig {
  using SignatureType = std::bitset<128>;
  using EIDGeneratorType =
      ushi::internal::impl::DefaultConfig::EIDGeneratorType;
};

struct NotAConfig {
  using Signature = ushi::u64;
};

using T_Config = ushi::internal::impl::DefaultConfig;
using T_EIDGen = ushi::internal::impl::EntityIDGenerator<ushi::u64>;
using T_Signature = T_Config::SignatureType;
using T_Entity = ushi::internal::impl::Entity<T_Config>;
using T_EntityFactory = ushi::internal::impl::EntityFactory<T_EIDGen>;
using T_EntityManager = ushi::internal::impl::EntityManager<T_Config>;
using T_Component = ushi::internal::impl::Component;
using T_Record = ushi::internal::impl::ComponentRecord<T_Config>;
using T_SystemManager = ushi::internal::impl::SystemManager<T_Config>;
using T_World = ushi::internal::impl::World<T_Config>;

using T_CustomEntity = ushi::internal::impl::Entity<CustomConfig>;
using T_CustomEntityManager = ushi::internal::impl::EntityManager<CustomConfig>;
using T_CustomRecord = ushi::internal::impl::ComponentRecord<CustomConfig>;
using T_CustomTable = ushi::internal::impl::ComponentTable<CustomConfig>;
using T_CustomGroup = ushi::internal::impl::Group<CustomConfig>;
using T_CustomGroupNetwork = ushi::internal::impl::GroupNetwork<CustomConfig>;
using T_CustomSystemManager = ushi::internal::impl::SystemManager<CustomConfig>;
using T_CustomWorld = ushi::internal::impl::World<CustomConfig>;

struct Motion : public ushi::internal::impl::Component {
  int value = 0;
  constexpr Motion() = default;
  constexpr Motion(int v) noexcept : value{v} {}
};

struct Assets : public ushi::internal::impl::Component {};

struct Animation : public Assets {
  int value = 0;
  constexpr Animation() = default;
  constexpr Animation(int v) noexcept : value{v} {}
};

struct Texture : public Assets {
  int value = 0;
  constexpr Texture() = default;
  constexpr Texture(int v) noexcept : value{v} {}
};

struct Audio : public Assets {
  int value = 0;
  constexpr Audio() = default;
  constexpr Audio(int v) noexcept : value{v} {}
};

#endif // USHI_TEST_TEST_UTILS_HPP