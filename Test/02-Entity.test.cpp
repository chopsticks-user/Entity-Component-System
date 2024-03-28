#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

using EPlayer = ushi::Entity<ushi::DefaultConfig>;
using EEnemy = ushi::Entity<ushi::DefaultConfig>;

using BitSet = ushi::DefaultConfig::SignatureType;

struct NotEntity : public ushi::EntityBase {};

TEST_CASE("Case #01: IsValidEntity and IsValidConfig", "[require]") {
  REQUIRE(ushi::IsValidEntity<EPlayer>);
  REQUIRE(ushi::IsValidEntity<EEnemy>);
  REQUIRE_FALSE(ushi::IsValidEntity<NotEntity>);
}

TEST_CASE("Case #02: EntityIDGenerator", "[require]") {
  ushi::EntityIDGenerator idGenerator;
  REQUIRE(idGenerator() == 0);
  REQUIRE(idGenerator() == 1);
  REQUIRE(idGenerator() == 2);
}

TEST_CASE("Case #03: EntityFactory", "[require]") {
  ushi::EntityFactory factory;

  // Implicitly convertible to ushi::EntityID
  REQUIRE(factory.create<EPlayer>() == 0ul);

  auto player1 = factory.create<EPlayer>();
  REQUIRE(player1.id() == 1ul);

  static constexpr BitSet bitset = 0xf1a0;

  // Clone
  auto player2 = factory.create<EPlayer>(bitset);
  REQUIRE(player2.id() == 2ul);
  REQUIRE(player2.signature() == bitset);

  auto player3 = factory.clone<EPlayer>(player2);
  REQUIRE(player3.id() == 3ul);
  REQUIRE(player3.signature() == bitset);

  // Create an Entity using a Config template
  auto player4 = factory.create<ushi::DefaultConfig>(player3.signature());
  REQUIRE(player4.id() == 4ul);
  REQUIRE(player4.signature() == bitset);
}
