#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

using EPlayer = ushi::Entity<ushi::DefaultConfig>;
using EEnemy = ushi::Entity<ushi::DefaultConfig>;

using Bitset = ushi::DefaultConfig::SignatureType;

struct CustomConfig {
  using SignatureType = std::bitset<128>;
};

struct NotAConfig {
  using Signature = ushi::u64;
};

template <typename T> //
struct Haha {};

template <typename T> struct Test {
  T value;
};

TEST_CASE("Case #01: Concepts", "[require]") {
  REQUIRE(ushi::IsValidConfig<ushi::DefaultConfig>);
  REQUIRE(ushi::IsValidConfig<CustomConfig>);
  REQUIRE(ushi::IsValidEntity<EPlayer>);
  REQUIRE(ushi::IsValidEntity<ushi::Entity<CustomConfig>>);
  REQUIRE_FALSE(ushi::IsValidConfig<NotAConfig>);
}

TEST_CASE("Case #02: EntityIDGenerator", "[require]") {
  ushi::EntityIDGenerator idGenerator;
  REQUIRE(idGenerator() == 0);
  REQUIRE(idGenerator() == 1);
  REQUIRE(idGenerator() == 2);
}

TEST_CASE("Case #03: EntityFactory", "[require]") {
  REQUIRE(ushi::IsValidEntity<EPlayer>);
  REQUIRE(ushi::IsValidEntity<EEnemy>);
  REQUIRE(ushi::IsValidEntity<ushi::Entity<CustomConfig>>);

  ushi::EntityFactory factory;

  // Implicitly convertible to ushi::EntityID
  REQUIRE(factory.create<EPlayer>() == 0ul);

  auto player1 = factory.create<EPlayer>();
  REQUIRE(player1.id() == 1ul);

  static constexpr Bitset bitset = 0xf1a0;

  // Clone
  auto player2 = factory.create<EPlayer>(bitset);
  REQUIRE(player2.id() == 2ul);
  REQUIRE(player2.signature() == bitset);

  auto player3 = factory.clone<EPlayer>(player2);
  REQUIRE(player3.id() == 3ul);
  REQUIRE(player3.signature() == bitset);

  // Create an Entity using a Config template
  auto player4 = factory.create<CustomConfig>();
  REQUIRE(player4.id() == 4ul);
  REQUIRE(player4.maxComponents() == CustomConfig::SignatureType{}.size());
}

TEST_CASE("Case #04: EntityFactory", "[require]") {}
