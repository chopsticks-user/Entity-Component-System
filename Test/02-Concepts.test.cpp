#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

using Entity = ushi::Entity<ushi::DefaultConfig>;
using Bitset = ushi::DefaultConfig::SignatureType;

struct CustomConfig {
  using SignatureType = std::bitset<128>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

struct NotAConfig {
  using Signature = ushi::u64;
};

TEST_CASE("Case #01: IsValidConfig", "[require]") {
  REQUIRE(ushi::IsValidConfig<ushi::DefaultConfig>);
  REQUIRE(ushi::IsValidConfig<CustomConfig>);
  REQUIRE_FALSE(ushi::IsValidConfig<NotAConfig>);
}

TEST_CASE("Case #02: IsValidEntity", "[require]") {
  REQUIRE(ushi::IsValidEntity<Entity>);
  REQUIRE(ushi::IsValidEntity<ushi::Entity<CustomConfig>>);
}