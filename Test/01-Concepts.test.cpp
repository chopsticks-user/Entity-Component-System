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

TEST_CASE("Case #01: IsConfig", "[require]") {
  REQUIRE(ushi::IsConfig<ushi::DefaultConfig>);
  REQUIRE(ushi::IsConfig<CustomConfig>);
  REQUIRE_FALSE(ushi::IsConfig<NotAConfig>);
}

TEST_CASE("Case #02: IsEntity", "[require]") {
  REQUIRE(ushi::IsEntity<Entity>);
  REQUIRE(ushi::IsEntity<ushi::Entity<CustomConfig>>);
}